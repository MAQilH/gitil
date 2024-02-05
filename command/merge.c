#include "diff.h"
#include "commit.h"
#include "branch.h"
#include "add.h"
#include "status.h"
#include "../lib/lib.h"
#include "../model/commit_model.h"
#include "../model/file_list_model.h"
#include "conflict.h"

int merge_validate(char *branch1, char *branch2){
    if(!check_exist_branch(branch1)){
        print_fail("fail: main branch not exist!");
        return 0;
    }
    if(!check_exist_branch(branch2)){
        print_fail("fail: sub branch not exist!");
        return 0;
    }
    if(!strcmp(branch1, branch2)){
        print_fail("fail: base and sub branch must be different!");
        return 0;
    }
    char *commit_id1 = get_branch_head_commit(branch1);
    char *commit_id2 = get_branch_head_commit(branch2);

    if(strcmp(get_current_commit(), commit_id1)){
        print_fail("fail: you must in the HEAD of main branch!");
        return 0;
    }
    if(check_diff_in_project()){
        print_fail("project have changes in files that not been committed, you can seen them with \"gitil status -p\"!");
        return 0;
    }
    return 1;
}

void add_sub_branch_file(char* branch1, char* branch2){
    char* commit_main = get_branch_head_commit(branch1);
    char* commit_sub = get_branch_head_commit(branch2);
    FileList* main_files = get_commit_status_file(commit_main);
    FileList* sub_files = get_commit_status_file(commit_sub);
    for(int i = 0; i < sub_files->cnt; i++){
        if(sub_files->lst[i].state == Delete) continue;
        if(find_index_in_file_list(main_files, sub_files->lst[i].addres) == -1 || find_in_file_list(main_files, sub_files->lst[i].addres) == Delete){
            file_copy(
                exist_in_commit(commit_sub, sub_files->lst[i].addres),
                sub_files->lst[i].addres
            );
        }
    }
}

int merge_branch(char* branch1, char* branch2){
    if(merge_validate(branch1, branch2)){
        if(!conflict(get_branch_head_commit(branch2))){
            print_fail("you must resolve conflicte and then merge this two branch!");
            return 0;
        }
        add_sub_branch_file(branch1, branch2);
        add_all_changes();
        char msg[MAX_ADDRES];
        sprintf(msg, "Merged Branch %s with %s!", branch1, branch2);
        create_commit(msg, 0, 1);
        return 1;
    }
    return 0;
}

int merge(int argc, char *argv[]){
    if(!strcmp(argv[2], "-b")){
        char *branch1 = argv[3], *branch2 = argv[4];
        if(merge_branch(branch1, branch2)){
            print_success("Merge was succesful!");
            return 1;
        }
        return 0;
    }
    return 0;
}
