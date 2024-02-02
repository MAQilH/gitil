#include "branch_model.h"
#include "../lib/stril.h"
#include <stdio.h>

void print_branch(Branch brn){
    if(brn.hidden) return;
    printf(
        "name: %s\n"
        "creator: %s\n"
        "parrent_branch: %s\n"
        "parrent_commit_id: %s\n"
        "head_commid_id: %s\n"
        "date: %s\n\n",
        brn.name, brn.creator, brn.parrent_branch, 
        brn.parent_commit_id, brn.head_commit_id, get_date(brn.date)
    );   
}

void print_branch_file(char* file_addres){
    FILE* file = fopen(file_addres, "rb");
    Branch tmp;
    while(fread(&tmp, sizeof(tmp), 1, file)){
        print_branch(tmp);
    }
    fclose(file);
}