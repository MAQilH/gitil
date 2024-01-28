#include "../model/branch_model.h"
#include "../lib/lib.h"
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
    
    create_folder(get_branch_folder_addres(branch_name));
    
    FILE* file = fopen(get_branch_folder_addres(branch_name), "wb");
    fwrite(&brn, sizeof(brn), 1, file);
    fclose(file);

    // char *stage_info_addres = cat_string(folder_addres, "\\stage_info.dat");
    // file = fopen(stage_info_addres, "wb");
    // fclose(file);

    // char *last_unstage_info_addres = cat_string(folder_addres, "\\last_unstage_info.dat");
    // file = fopen(last_unstage_info_addres, "wb");
    // fclose(file);

    file = fopen(get_commit_info_addres(branch_name), "wb");
    fclose(file);

    file = fopen(get_stage_info_addres(branch_name), "wb");
    fclose(file);

    file = fopen(get_unstage_info_addres(branch_name), "wb");
    fclose(file);
}

void create_master_branck(){

}