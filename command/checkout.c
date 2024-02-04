#include "../lib/lib.h"
#include "../model/file_list_model.h"
#include "commit.h"
#include "status.h"
#include "branch.h"
#include "../model/branch_model.h"
#include "revert.h"

int validate_checkout(char* commit_id){
    if(check_diff_in_project()){
        print_fail("project have changes in files that not been committed, you can seen them with \"gitil status -p\"!");
        return 0;
    }
    print_success("checkout has been successfully!");
    return 1;
}

int checkout_commit(char* commit_id, int force){
    if(!force && !validate_checkout(commit_id)) return 0;
    if(!revert_n(commit_id)) return 0;
    set_current_commit(commit_id);
    return 1;
}

void checkout_branch(char* branch_name, int force){
    char *head_commit = get_branch_head_commit(branch_name);
    if(checkout_commit(head_commit, force)){
        set_HEAD_branch(branch_name);
    }
}

char* get_HEAD_x_commit_id(int n){
    char* commit_id = get_cuurent_HEAD_commit();
    Branch current_branch = get_branch(get_HEAD());
    while(n--){
        char* prev_commit = get_prev_commit_id(commit_id);
        if(prev_commit == NULL || !strcmp(prev_commit, current_branch.parent_commit_id)) break;
        commit_id = prev_commit;
    }
    return commit_id;
}

void checkout_head_n(int n, int force){
    checkout_commit(get_HEAD_x_commit_id(n), force);
}

int checkout(int argc, char* argv[]){
    int force = !strcmp(argv[argc-1], "-f");
    if(!strcmp("HEAD", argv[2])){
        checkout_head_n(0, force);
        return 1;
    }
    if(!strcmp("__CUR", argv[2])){
        checkout_commit(get_current_commit(), 1);
        return 1;
    }
    if(strlen(argv[2]) > 5 && !strncmp("HEAD-", argv[2], 5)){
        checkout_head_n(stoi(argv[2] + 5), force);
        return 1;
    }
    if(check_exist_branch(argv[2])){
        checkout_branch(argv[2], force);
        return 1;
    } 
    if(check_exist_commit(argv[2])){
        checkout_commit(argv[2], force);
        return 1;
    }
    print_fail("not exist such commit/branch!");
    return 0;
}