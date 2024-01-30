#include "../model/branch_model.h"
#include "../model/commit_model.h"
#include "../model/file_list_model.h"
#include "../lib/lib.h"
#include "commit.h"
#include <string.h>
#include <stdio.h>

void create_branch(char *branch_name){
    Branch brn;
    strcpy(brn.creator, get_creator());
    strcpy(brn.name, branch_name);
    if(!strcmp(branch_name, "master")){
        strcpy(brn.parrent_branch, "master");
        strcpy(brn.parent_commit_id, "master");
        strcpy(brn.head_commit_id, "master");
    } else{
        strcpy(brn.parrent_branch, get_commit_branch(get_current_commit()));
        strcpy(brn.parent_commit_id, get_current_commit());
        strcpy(brn.head_commit_id, get_current_commit());
    }
    brn.date = time(NULL);


    create_folder(get_branch_folder_addres(branch_name));
    create_folder(get_commits_folder_addres(branch_name));
    create_folder(get_stage_changes_folder_addres(branch_name));
    
    FILE* file = fopen(get_branch_info_addres(), "ab");
    fwrite(&brn, sizeof(brn), 1, file);
    fclose(file);

    file = fopen(get_commit_info_addres(branch_name), "wb");
    fclose(file);

    FileList flst = {.cnt = 0};
    set_file_list(get_mem_stage_info_addres(branch_name), &flst);
    set_file_list(get_stage_info_addres(branch_name), &flst);

    file = fopen(get_unstage_info_addres(branch_name), "wb");
    fclose(file);

    set_HEAD_branch(branch_name);
    if(!strcmp(branch_name, "master")){
        create_commit("gitil added!");
    }
}

Branch get_branch_and_index(char* branch_name, int *index){
    FILE *branch_info_file = fopen(get_branch_info_addres(), "rb");
    Branch brn;
    *index = 0;
    int have = 0;
    while(fread(&brn, sizeof(brn), 1, branch_info_file)){
        if(!strcmp(brn.name, branch_name)){
            have = 1;
            break;
        }
        *index++;
    }
    fclose(branch_info_file);
    if(!have) *index = -1;
    return brn;
}

Branch get_branch(char* branch_name){
    FILE *branch_info_file = fopen(get_branch_info_addres(), "rb");
    Branch brn;
    while(fread(&brn, sizeof(brn), 1, branch_info_file)){
        if(!strcmp(brn.name, branch_name)){
            break;
        }
    }
    fclose(branch_info_file);
    return brn;
}

int validate_branch(char* branch_name, char *res){
    FILE* file = fopen(get_branch_info_addres(), "rb");
    Branch brn;
    while(fread(&brn, sizeof(brn), 1, file)){
        if(!strcmp(brn.name, branch_name)){
            res = "fail: name is duplicated!";
            return 0;
        }
    }

    res = "Branch ";
    res = cat_string(res, branch_name);
    res = cat_string(res, " created!");
    return 1;
}

void show_branchs(){
    FILE* file = fopen(get_branch_info_addres(), "rb");
    Branch brn;
    while(fread(&brn, sizeof(brn), 1, file)){
        print_branch(brn);
    }
}

void branch(int argc, char *argv[]){
    if(argc == 2){
        show_branchs();
    } else{
        char* msg = (char*)calloc(MAX_MESSAGE, sizeof(char));
        if(validate_branch(argv[2], msg)){
            create_branch(argv[2]);
            print_success(msg);
            return;
        }
        print_fail(msg);
    }
}