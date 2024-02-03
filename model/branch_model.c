#include "branch_model.h"
#include "../lib/stril.h"
#include "../lib/data_base.h"
#include <stdio.h>
#include <string.h>

void print_branch(Branch brn){
    if(brn.hidden) return;
    char* is_head = strcmp(get_HEAD(), brn.name)? "": "\033[1;34m(HEAD)\033[0m"; 
    printf(
        "\e[1mName:\e[m %s %s\n"
        "\e[1mCreator:\e[m %s\n"
        "\e[1mParrent Branch:\e[m %s\n"
        "\e[1mParrent ID:\e[m \033[1;33m%s\033[0m\n"
        "\e[1mHead ID:\e[m \033[1;33m%s\033[0m\n"
        "\e[1mDate:\e[m %s\n\n",
        brn.name, is_head, 
        brn.creator, brn.parrent_branch, 
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