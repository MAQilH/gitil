#include "../lib/lib.h"
#include "../model/file_list_model.h"
#include "commit.h"
#include "status.h"
#include "branch.h"
#include "../model/branch_model.h"

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
    FileList flst = {.cnt = 0};
    get_file_status_with_commit(commit_id, &flst, get_root_addres(), MAX_DEP);
    for(int i = 0; i < flst.cnt; i++){
        if(flst.lst[i].state == Create){
            remove(flst.lst[i].addres);
        } else if(flst.lst[i].state != Unchange){
            char *copy_addres = exist_in_commit(commit_id, flst.lst[i].addres);
            if(copy_addres == NULL){
                print_error("in checkout_commit copy addres not found!");
                return 0;
            }
            file_copy(
                exist_in_commit(commit_id, flst.lst[i].addres),
                flst.lst[i].addres
            );
        }
    }
    // set_HEAD_branch(get_commit_branch(commit_id));
    set_current_commit(commit_id);
    return 1;
}

void checkout_branch(char* branch_name, int force){
    char *head_commit = get_branch_head_commit(branch_name);
    if(checkout_commit(head_commit, force)){
        set_HEAD_branch(branch_name);
    }
}

void checkout_head_n(int n){
    char* commit_id = get_cuurent_HEAD_commit();
    Branch current_branch = get_branch(get_HEAD());
    while(n--){
        char* prev_commit = get_prev_commit_id(commit_id);
        if(prev_commit == NULL || !strcmp(prev_commit, current_branch.parent_commit_id)) break;
        commit_id = prev_commit;
    }
    checkout_commit(commit_id, 0);
}

int checkout(int argc, char* argv[]){
    if(!strcmp("HEAD", argv[2])){
        checkout_head_n(0);
        return 1;
    }
    if(strlen(argv[2]) > 5 && !strncmp("HEAD-", argv[2], 5)){
        checkout_head_n(stoi(argv[2] + 5));
        return 1;
    }
    if(check_exist_branch(argv[2])){
        checkout_branch(argv[2], 0);
        return 1;
    } 
    if(check_exist_commit(argv[2])){
        checkout_commit(argv[2], 0);
        return 1;
    }
    print_fail("not exist such commit/branch!");
    return 0;
}