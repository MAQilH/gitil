#include "../lib/lib.h"
#include "../model/commit_model.h"
#include "../model/file_list_model.h"
#include "../model/branch_model.h"

char* exist_in_commit(char *commit_id, char* file_addres){
    State sts = find_in_file_list_with_addres(
        get_commit_status_file_addres(commit_id),
        file_addres
    );
    if(sts == Delete || sts == NotFound) return NULL; 
    int index = find_index_in_file_list_with_addres(
        get_commit_status_file_addres(commit_id),
        file_addres
    );
    return replace_name_with_id(file_addres, index);
}

State get_file_state_with_commit(char *commit_id, char* file_addres){
    char* addres_in_commit = exist_in_commit(commit_id, file_addres);
    if(!exist_file(file_addres)){
        if(addres_in_commit == NULL) return NotFound;
        return Delete;
    }
    if(addres_in_commit == NULL) return Create;
    if(file_cmp(addres_in_commit, file_addres)) return Unchange;
    return Modified;
}

FileList get_commit_status_file(char *commit_id){
    FILE *file = fopen(get_commit_status_file_addres(commit_id), "rb");
    FileList flst;
    fread(&flst, sizeof(flst), 1, file);
    fclose(file);
    return flst; 
}

char* create_random_commit_id(){
    srand(time(NULL));
    int LEN = 20;
    char *res = (char*)calloc(LEN, 1);
    for(int i = 0; i < LEN; i++){
        res[i] = (char)('a' + rand()%26);
    }
    return res;
}

void append_commit(Commit cmt){
    FILE *commit_info_file = fopen(get_current_commit_info_addres(), "ab");
    fwrite(&cmt, sizeof(cmt), 1, commit_info_file);
    fclose(commit_info_file);
    return;
}

void update_head_commit_id(char* commit_id){
    int index;
    Branch brn = get_branch_and_index(get_HEAD(), &index);
    strcpy(brn.head_commit_id, commit_id);

    upate_middle_file(&brn, sizeof(brn), index, get_branch_info_addres());
}

void create_commit_init_file(char* commit_id){
    create_folder(get_commit_folder_addres(commit_id));
    FILE *status_file = fopen(get_commit_status_file_addres(commit_id), "wb");
    FileList empt = {.cnt = 0};
    fwrite(&empt, sizeof(empt), 1, status_file);
    fclose(status_file);
}

void push_stage(){
    
}

void create_commit(char* message){
    char *commit_id = create_random_commit_id();
    Commit cmt;
    strcpy(cmt.message, message);
    strcpy(cmt.creator, get_creator());
    strcpy(cmt.branch_name, get_HEAD());
    strcpy(cmt.commit_id, commit_id);
    cmt.date = time(NULL);
    
    append_commit(cmt);
    update_head_commit_id(commit_id);
    push_stage(commit_id);
    create_commit_init_file(commit_id);
}