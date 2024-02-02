#include "../lib/lib.h"
#include "../model/commit_model.h"
#include "../model/file_list_model.h"
#include "../model/branch_model.h"
#include "branch.h"
#include "commit_shortcut.h"
#include "diff.h"
#include "stage.h"

int check_exist_commit(char* commit_id){
    FILE *commits_info_file = fopen(get_commits_info_addres(), "rb");
    Commit cmt;
    while(fread(&cmt, sizeof(cmt), 1, commits_info_file)){
        if(!strcmp(cmt.commit_id, commit_id)){
            fclose(commits_info_file);
            return 1;
        }
    }
    fclose(commits_info_file);
    return 0;
}

char* get_prev_commit_id(char* commit_id){
    if(commit_id == NULL) return NULL;
    char* branch_name = get_commit_branch(commit_id);
    FILE *commits_info_file = fopen(get_commits_info_addres(), "rb");
    Commit last_cmt;
    fread(&last_cmt, sizeof(last_cmt), 1, commits_info_file);
    if(!strcmp(last_cmt.commit_id, commit_id)){
        fclose(commits_info_file);
        if(!strcmp(branch_name, "master")) return NULL;
        Branch brn = get_branch(branch_name);
        return get_string_ref(brn.parent_commit_id);
    }
    Commit cmt;
    while(fread(&cmt, sizeof(cmt), 1, commits_info_file)){
        if(!strcmp(cmt.commit_id, commit_id)){
            break;
        }
        last_cmt = cmt;
    }
    fclose(commits_info_file);
    return get_string_ref(last_cmt.commit_id);
}

char* exist_in_commit(char *commit_id, char* file_addres){
    if(commit_id == NULL || !strcmp(commit_id, "master")) return NULL;
    State sts = find_in_file_list_with_addres(
        get_commit_status_file_addres(commit_id),
        file_addres
    );
    if(sts == Delete) return NULL;
    if(sts == NotFound){
        return exist_in_commit(get_prev_commit_id(commit_id), file_addres);
    } 
    return get_commit_saved_file_addres(commit_id, file_addres);
}

State get_file_state_with_commit(char *commit_id, char* file_addres){ // TODO: ino baiad bebarm to diff
    return get_changed_state(exist_in_commit(commit_id, file_addres), file_addres);
}

FileList get_commit_status_file(char *commit_id){
    return get_file_list(get_commit_status_file_addres(commit_id)); 
}

char* create_random_commit_id(){
    srand(get_rand());
    char *res = get_string(MAX_HASH);
    for(int i = 0; i < MAX_HASH; i++){
        res[i] = (char)('a' + rand()%26);
    }
    return res;
}

void append_commit(Commit cmt){
    FILE *commit_info_file = fopen(get_current_commit_info_addres(), "ab");
    fwrite(&cmt, sizeof(cmt), 1, commit_info_file);
    fclose(commit_info_file);

    FILE *commits_info_file = fopen(get_commits_info_addres(), "ab");
    fwrite(&cmt, sizeof(cmt), 1, commit_info_file);
    fclose(commits_info_file);
    return;
}

void update_head_commit_id(char* commit_id){
    int index = get_branch_index(get_HEAD());
    Branch brn = get_branch(get_HEAD());
    strcpy(brn.head_commit_id, commit_id);

    upate_middle_file(&brn, sizeof(brn), index, get_branch_info_addres());
    set_current_commit(commit_id);
}

void create_commit_init_file(char* commit_id){
    create_folder(get_commit_folder_addres(commit_id));
    FileList flst = {.cnt = 0};
    set_file_list(get_commit_status_file_addres(commit_id), &flst);
}

void clear_stage(){
    FileList stage_file = get_file_list(get_current_stage_info_addres());
    stage_file = get_clean_file_list(&stage_file);
    for(int i = 0; i < stage_file.cnt; i++){
        remove_file_in_stage_change(&stage_file.lst[i]);
    }
    FileList flst = {.cnt = 0};
    set_file_list(get_current_mem_stage_info_addres(), &flst);
    set_file_list(get_current_stage_info_addres(), &flst);
    FILE *unstage_file = fopen(get_current_unstage_info_addres(), "wb");
    fclose(unstage_file);
}

void push_stage(char* commit_id){
    FileList stage_file = get_file_list(get_current_stage_info_addres());
    FileList file_status = get_clean_file_list(&stage_file);

    char* current_commit = get_current_commit();
    for(int i = 0; i < file_status.cnt; i++){
        file_copy(
            get_current_stage_changes_file_addres(get_name_file_in_stage_change(file_status.lst[i].addres)),
            get_commit_saved_file_addres(commit_id, file_status.lst[i].addres)
        );
    }

    FileList all_project_file = {.cnt = 0};
    get_file_status(&all_project_file, get_root_addres(), MAX_DEP);
    for(int i = 0; i < all_project_file.cnt; i++){

        if(find_index_in_file_list(&file_status, all_project_file.lst[i].addres) == -1){
            all_project_file.lst[i].state = NotFound;
            file_status.lst[file_status.cnt++] = all_project_file.lst[i];
        }
    }
    set_file_list(get_commit_status_file_addres(commit_id), &file_status);

    clear_stage();
}

Commit create_commit(char *message, int hidden){
    char *commit_id = create_random_commit_id();
    Commit cmt;
    strcpy(cmt.message, message);
    strcpy(cmt.creator, get_creator());
    strcpy(cmt.branch_name, get_HEAD());
    strcpy(cmt.commit_id, commit_id);
    cmt.date = time(NULL);
    cmt.hidden = hidden;

    append_commit(cmt);
    create_commit_init_file(commit_id);
    push_stage(commit_id);
    update_head_commit_id(commit_id);

    return cmt;
}

int validate_create_commit(char* message, char* res){
    FileList stage_list = get_file_list(get_current_stage_info_addres());
    stage_list = get_clean_file_list(&stage_list);
    if(stage_list.cnt == 0){
        strcpy(res, "stage area is empty!");
        return 0;
    }
    if(strlen(message) > 72){
        strcpy(res, "message size if more than 72 character!");
        return 0;
    }

    if(strcmp(get_branch_head_commit(get_HEAD()), get_current_commit())){
        strcpy(res, "you are not in head project!");
        return 0;
    }

    strcpy(res, "commit created!");
    return 1; 
}

int get_number_commited_file(char* commit_id){
    FileList status_file = get_commit_status_file(commit_id);
    int cnt = 0;
    for(int i = 0; i < status_file.cnt; i++){
        if(status_file.lst[i].state == NotFound) break; 
        if(status_file.lst[i].state != Unchange) cnt++;
    }
    return cnt;
}

int commit(int argc, char *argv[]){
    if(argc < 4){
        print_input_invalid();
        return 0;
    }
    char* commit_msg = argv[3];
    if(!strcmp(argv[2], "-s")){
        commit_msg = get_commit_shortcut(argv[3]);
        if(commit_msg == NULL){
            print_fail("fail: shortcut not founded!");
            return 0;
        }
    }
    if(!strcmp(argv[2], "-m") || !strcmp(argv[2], "-s")){
        char* msg = get_string(MAX_MESSAGE);
        if(validate_create_commit(commit_msg, msg)){
            Commit cmt = create_commit(commit_msg, 0);
            print_success(msg);
            print_commit(cmt);
        } else{
            print_fail(msg);
            return 0;
        }
        return 1;
    }
    print_input_invalid();
    return 0;
}