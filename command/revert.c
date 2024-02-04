#include "commit.h"
#include "status.h"
#include "add.h"
#include "checkout.h"
#include "status.h"
#include "../model/file_list_model.h"
#include "../lib/lib.h"
#include "../model/commit_model.h"

int revert_n(char* commit_id){
    FileList *flst = create_file_list(0);
    get_file_status_with_commit(commit_id, flst, get_root_addres(), MAX_DEP);
    for(int i = 0; i < flst->cnt; i++){
        if(flst->lst[i].state == Create){
            remove(flst->lst[i].addres);
        } else if(flst->lst[i].state != Unchange){
            char *copy_addres = exist_in_commit(commit_id, flst->lst[i].addres);
            if(copy_addres == NULL){
                print_error("in checkout_commit copy addres not found!");
                return 0;
            }
            file_copy(
                copy_addres,
                flst->lst[i].addres
            );
        }
    }
    return 1;
}

int revert_n_commit_id(char* commit_id){
    Commit cmt = get_commit(commit_id);
    if(cmt.is_merged){
        print_fail("fail: this commite created by merge!");
        return 0;
    }
    return revert_n(get_prev_commit_id(commit_id));
}

int revert_commit_id(char* commit_id, char* msg){
    Commit cmt = get_commit(commit_id);
    if(cmt.is_merged){
        print_fail("fail: this commite created by merge!");
        return 0;
    }
    commit_id = get_prev_commit_id(commit_id);
    cmt = get_commit(commit_id);
    if(msg == NULL){
        msg = get_string(MAX_MESSAGE);
        strcpy(msg, cmt.message);
    }
    if(!revert_n(commit_id)) return 0;
    add_all_changes();
    create_commit(msg, 0, 0);
}

char* get_last_commit_id(){
    FILE *commits_info = fopen(get_commits_info_addres(), "rb");
    Commit cmt;
    char *commit_id = get_string(MAX_HASH);
    while(fread(&cmt, sizeof(cmt), 1, commits_info)){
        if(cmt.hidden == 0){
            strcpy(commit_id, cmt.commit_id);
        }
    }
    return commit_id;
}

int revert(int argc, char *argv[]){
    if(check_diff_in_project()){
        print_fail("project have changes in files that not been committed, you can seen them with \"gitil status -p\"!");
        return 0;
    }
    if(strcmp(get_current_commit(), get_cuurent_HEAD_commit())){
        print_fail("fail: you most be in the HEAD of project!");
        return 0;
    }
    if(!strcmp(argv[2], "-n")){
        char *commit_id = get_last_commit_id();
        if(argc > 3){
            commit_id = argv[3];
        }
        revert_n_commit_id(commit_id);
        return 1;   
    }
    char *msg = NULL;
    if(!strcmp(argv[2], "-m")){
        msg = argv[3];
    }
    char *commit_id = argv[argc-1];
    if(!strncmp(commit_id, "HEAD-", 5)){
        commit_id = get_HEAD_x_commit_id(stoi(commit_id + 5));
    }
    return revert_commit_id(commit_id, msg);
}