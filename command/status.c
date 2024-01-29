#include "../model/file_list_model.h"
#include "../lib/lib.h"

void dfs_get_file(FileList *flst, int dep, char* commit_id){ 
    if(dep == 0) {
        return;
    }
    DIR *dir = opendir(".");
    struct dirent *ent;
    while((ent = readdir(dir)) != NULL){
        if(!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..")) continue;
        if(is_directory(ent->d_name)){
            chdir(ent->d_name);
            dfs_get_file(flst, dep-1, commit_id);
            chdir("..");
        } else{
            char *file_addres = cat_string(get_current_addres(), ent->d_name);
            File fl;
            strcpy(fl.addres, file_addres);
            strcpy(fl.name, ent->d_name);
            fl.state = get_file_state_with_commit(commit_id, file_addres);

            flst->lst[flst->cnt++] = fl;
        }
    }
    closedir(dir);
    return;
}

void get_file_status(FileList *flst, char* folder_addres){
    char* saved = get_current_addres();
    chdir(folder_addres);
    dfs_get_file(flst, MAX_DEP, get_cuurent_HEAD_commit());
    FileList cmt_status_file = get_commit_status_file(get_cuurent_HEAD_commit());
    for(int i = 0; i < cmt_status_file.cnt; i++){
        if(
            check_exist_in_folder(cmt_status_file.lst[i].addres, folder_addres) &&
            cmt_status_file.lst[i].state != Delete &&
            find_index_in_file_list(flst, cmt_status_file.lst[i].addres) == -1
        ){
            cmt_status_file.lst[i].state = Delete;
            flst->lst[flst->cnt++] = cmt_status_file.lst[i];
        }
    } 
    chdir(saved);
    return;
}




