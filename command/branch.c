#include "../model/branch_model.h"
#include "../lib/lib.h"
#include "commit.h"
#include <string.h>
#include <stdio.h>

void create_branch(char *branch_name, char *parent_branch_name){
    Branch brn;
    strcpy(brn.creator, get_creator());
    strcpy(brn.name, branch_name);
    strcpy(brn.parrent_branch, parent_branch_name);
    if(!strcmp(branch_name, "master")){
        strcpy(brn.parent_commit_id, "master");
        strcpy(brn.head_commit_id, "master");
    } else{
        strcpy(brn.parent_commit_id, get_branch_head_commit(parent_branch_name));
        strcpy(brn.head_commit_id, get_branch_head_commit(parent_branch_name));
    }
    brn.date = time(NULL);
    
    create_folder(get_branch_folder_addres(branch_name));
    create_folder(get_commits_folder_addres(branch_name));
    
    FILE* file = fopen(get_branch_info_addres(), "ab");
    fwrite(&brn, sizeof(brn), 1, file);
    fclose(file);

    file = fopen(get_commit_info_addres(branch_name), "wb");
    fclose(file);

    file = fopen(get_stage_info_addres(branch_name), "wb");
    fclose(file);

    file = fopen(get_unstage_info_addres(branch_name), "wb");
    fclose(file);

    if(!strcmp(branch_name, "master")){
        create_commit("gitil add to project weee XD");
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