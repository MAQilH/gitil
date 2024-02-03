#include "../model/file_list_model.h"
#include "../lib/lib.h"
#include "stage.h"
#include <sys/stat.h>

void dfs_get_file(FileList *flst, int dep, char* commit_id){ 
    if(dep == 0) {
        return;
    }
    DIR *dir = opendir(".");
    struct dirent *ent;
    while((ent = readdir(dir)) != NULL){
        if(!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..") || !strcmp(ent->d_name, ".gitil")) continue;
        if(is_directory(ent->d_name)){
            chdir(ent->d_name);
            dfs_get_file(flst, dep-1, commit_id);
            chdir("..");
        } else{
            char *file_addres = cat_string(get_current_addres(), ent->d_name);
            File *fl = malloc(sizeof(File));
            strcpy(fl->addres, file_addres);
            strcpy(fl->name, ent->d_name);
            fl->state = get_file_state_with_commit(commit_id, file_addres);
            file_list_push_back(flst, fl);
            free(fl);
        }
    }
    closedir(dir);
    return;
}

void get_file_status(FileList *flst, char* folder_addres, int dep){
    get_file_status_with_commit(get_current_commit(), flst, folder_addres, dep);
    return;
}

void get_file_status_with_commit(char* commit_id, FileList *flst, char* folder_addres, int dep){
    char* saved = get_current_addres();
    if(chdir(folder_addres) == 0){
        dfs_get_file(flst, dep, commit_id);
        chdir(saved);
    }
    FileList *cmt_status_file = get_commit_status_file(commit_id);
    for(int i = 0; i < cmt_status_file->cnt; i++){
        if(
            addres_distance(cmt_status_file->lst[i].addres, folder_addres) <= dep &&
            cmt_status_file->lst[i].state != Delete &&
            find_index_in_file_list(flst, cmt_status_file->lst[i].addres) == -1
        ){
            cmt_status_file->lst[i].state = Delete;
            file_list_push_back(flst, &cmt_status_file->lst[i]);
        }
    } 
    return;
}

int changed_file_stage_area(char* file_addres){
    State st = get_changed_state(get_current_stage_changes_file_addres(get_name_file_in_stage_change(file_addres)), file_addres);
    return st != NotFound && st != Unchange;
}

void status(int argc, char *argv[]){ // if add -p addres show from root project
    FileList *flst = create_file_list(0);
    char* addres;
    if(argc > 2 && !strcmp(argv[2], "-p")){
        get_file_status(flst, addres = get_root_addres(), MAX_DEP);
    } else{
        get_file_status(flst, addres = get_current_addres(), 1);
    }
    int n = 0;
    for(int i = 0; i < flst->cnt; i++){
        char st[MAX_NAME] = "+A  ";
        if(flst->lst[i].state == Modified) st[1] = 'M';
        else if(flst->lst[i].state == Delete) st[1] = 'D';
        else if(flst->lst[i].state == Create) st[1] = 'A';
        else if(flst->lst[i].state == Access) st[1] = 'T';
        // else if(flst.lst[i].state == Unchange) st[1] = 'U';
        else continue;
        n++;
        
        if(!in_stage(flst->lst[i].addres) || changed_file_stage_area(flst->lst[i].addres)){
            st[0] = '-';
            char *msg = cat_string(st, get_rel_addres_from(flst->lst[i].addres, addres));
            print_fail(msg);
        } else{
            st[0] = '+';
            char *msg = cat_string(st, get_rel_addres_from(flst->lst[i].addres, addres));
            print_success(msg);
        }
    }
    if(n == 0){
        print_warn("there is no changes in project!\n");
    }
}

int check_diff_in_project(){
    FileList *flst = create_file_list(0);
    get_file_status(flst, get_root_addres(), MAX_DEP);
    for(int i = 0; i < flst->cnt; i++){
        if(flst->lst[i].state != NotFound && flst->lst[i].state != Unchange) return 1;
    }
    return 0;
}