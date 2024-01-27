#include "../model/branch_model.h"
#include "lib.h"
#include <string.h>
#include <stdio.h>

void create_branch(char *branch_name, char *parent_branch_name, int parent_commit_id){
    Branch brn;
    strcpy(brn.creator, get_creator());
    strcpy(brn.name, branch_name);
    strcpy(brn.parrent_branch, parent_branch_name);
    brn.date = time(NULL);
    brn.parent_commit_id = parent_commit_id;
    brn.head_commit_id = 0;
    
    char *folder_addres = cat_string(get_branch_folder_addres(), cat_string("\\", branch_name));
    create_folder(folder_addres);
    
    char *branch_info_addres = cat_string(folder_addres, "\\branch_info.dat");
    FILE* file = fopen(branch_info_addres, "wb");
    fwrite(&brn, sizeof(brn), 1, file);
    fclose(file);

    // char *stage_info_addres = cat_string(folder_addres, "\\stage_info.dat");
    // file = fopen(stage_info_addres, "wb");
    // fclose(file);

    // char *last_unstage_info_addres = cat_string(folder_addres, "\\last_unstage_info.dat");
    // file = fopen(last_unstage_info_addres, "wb");
    // fclose(file);

    char *commit_info_addres = cat_string(folder_addres, "\\commit_info.dat");
    FILE* file = fopen(commit_info_addres, "wb");
    fclose(file);
}