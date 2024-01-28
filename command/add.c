#include "../lib/lib.h"
#include "add.h"
#include "../model/string_list_model.h"
#include "../model/file_list_model.h"

void add_file_list_to_stage(FileList stl){
    FILE* stage_file = fopen(get_current_stage_info_addres(), "ab");
    fwrite(&stl, sizeof(stl), 1, stage_file);
    fclose(stage_file);
}

void add_file_to_stage(File fl){
    FileList flst = {.cnt = 1};
    flst.lst[0] = fl;
    add_file_list_to_stage(flst);
}

void add_file_rel_addres_to_stage(char* rel_addres){
    File fl = {.state = Create};
    char* addres = get_current_addres();
    addres = cat_string(addres, rel_addres);
    strcpy(fl.addres, addres);
    add_file_to_stage(fl);
}

void add_file_addres_to_stage(char* addres){
    File fl = {.state = Create};
    strcpy(fl.addres, addres);
    add_file_to_stage(fl);
}

FileList dfs_get_file(int dep, char* commit_id){ 
    if(dep == 0) {
        FileList empt;
        empt.cnt = 0;
        return empt;
    }
    DIR *dir = opendir(".");
    struct dirent *ent;
    FileList res;
    while((ent = readdir(dir)) != NULL){
        if(is_directory(ent->d_name)){
            chdir(ent->d_name);
            FileList flst = dfs_get_file(dep-1, commit_id);
            merge_file_list(&res, &flst);
            chdir("..");
        } else{
            char *file_addres = cat_string(get_current_addres(), ent->d_name);
            File fl;
            strcpy(fl.addres, file_addres);
            strcpy(fl.name, ent->d_name);
            fl.state = get_file_state_with_commit(commit_id, file_addres);
            res.lst[res.cnt++] = fl;
        }
    }
    closedir(dir);
    return res;
}

void add_folder_addres_to_stage(char* rel_addres){
    char *saved = get_current_addres();
    chdir(rel_addres);
    FileList flist = dfs_get_file(MAX_DEP, get_cuurent_HEAD_commit());
    chdir(saved);
    add_file_list_to_stage(flist);
}

void add(int argc, char *argv[]){
    int ptr = 2;
    if(!strcmp(argv[ptr], "-f")){
        ptr++;
    }
    for(; ptr < argc; ptr++){
        if(is_directory(argv[ptr])){
            add_folder_addres_to_stage(argv[ptr]);
            print_success("folder added!");
        } else if(exist_file(argv[ptr])){
            add_file_rel_addres_to_stage(argv[ptr]);
            print_success("file added!");
        } else{
            print_warn("warn: no file/folder exist!");
        }
    }
}