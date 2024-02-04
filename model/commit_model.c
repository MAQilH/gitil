#include "commit_model.h"
#include "../command/commit.h"
#include "../lib/stril.h"
#include "../lib/data_base.h"
#include <stdio.h>
#include <string.h>

void print_stash(Commit sts){
    printf(
        "\e[1mID:\e[m \033[1;33m%s\033[0m\n"
        "\e[1mMessage:\e[m %s\n"
        "\e[1mBranch:\e[m %s\n"
        "\e[1mCommit Creator:\e[m \033[1;33m%s\033[0m\n"
        "\e[1mNumber:\e[m %d\n\n",
        sts.commit_id, sts.message, sts.branch_name, sts.creator, sts.date
    );
}

void print_commit(Commit cmt){
    if(cmt.hidden) return;
    char* is_cur = strcmp(get_current_commit(), cmt.commit_id)? 
        (strcmp(get_cuurent_HEAD_commit(), cmt.commit_id)? "": "\033[1;34m(HEAD)\033[0m"):
        (strcmp(get_cuurent_HEAD_commit(), cmt.commit_id)? "\033[1;34m(CUR)\033[0m": "\033[1;34m(HEAD, CUR)\033[0m"); 
    printf(
        "\e[1mID:\e[m \033[1;33m%s\033[0m %s\n"
        "\e[1mMessage:\e[m %s\n"
        "\e[1mBranch:\e[m %s\n"
        "\e[1mCreator:\e[m %s\n"
        "\e[1mN. Changes:\e[m %d\n"
        "\e[1mDate:\e[m %s\n\n",
        cmt.commit_id, is_cur, 
        cmt.message, cmt.branch_name, cmt.creator, get_number_commited_file(cmt.commit_id), get_date(cmt.date)
    );
}

void print_commit_file(char* file_addres){
    FILE* file = fopen(file_addres, "rb");
    Commit tmp;
    while(fread(&tmp, sizeof(tmp), 1, file)){
        print_commit(tmp);
    }
    fclose(file);
}