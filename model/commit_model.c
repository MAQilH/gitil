#include "commit_model.h"
#include "../command/commit.h"
#include "../lib/stril.h"
#include <stdio.h>

void print_commit(Commit cmt){
    printf(
        "id: %s\n"
        "message: %s\n"
        "branch: %s\n"
        "creator: %s\n"
        "changes: %d\n"
        "date: %s\n\n",
        cmt.commit_id, cmt.message, cmt.branch_name, cmt.creator, get_number_commited_file(cmt.commit_id), get_date(cmt.date)
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