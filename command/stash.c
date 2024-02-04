#include "commit.h"
#include "branch.h"
#include "checkout.h"
#include "add.h"
#include "diff.h"
#include "merge.h"
#include "status.h"
#include "../lib/lib.h"
#include "../model/commit_model.h"
#include "../model/branch_model.h"

Commit get_stash(int num, int *index){
    FILE* file = fopen(get_stash_info_addres(), "rb");
    fseek(file, 0, SEEK_END);
    int ptr = ftell(file);
    Commit sts, res;
    int ind = 0;
    while(ptr > 0){
        ind++;
        ptr -= sizeof(sts);
        fseek(file, ptr, SEEK_SET);
        fread(&sts, sizeof(sts), 1, file);
        if(sts.date < 0) continue;
        res = sts;
        *index = ind-1;
        if(num == 0) break;
        num--;
    }
    fclose(file);
    return res;
}

void add_to_stash_info(Commit *sts){
    FILE* stage_info_file = fopen(get_stash_info_addres(), "ab");
    fwrite(sts, sizeof(*sts), 1, stage_info_file);
    fclose(stage_info_file);
}

void stash_push(char *msg){
    int len = get_file_len(get_stash_info_addres(), sizeof(Commit));
    char *stash_name = "__stash__";
    stash_name = cat_string(stash_name, itos(len));
    char* current_branch = get_HEAD();
    char* current_commit = get_current_commit();

    create_branch(stash_name, 1);
    set_HEAD_branch(stash_name);
    add_all_changes();
    Commit sts = create_commit(msg, 1, 0);
    strcpy(sts.creator, current_commit);
    add_to_stash_info(&sts);

    checkout_branch(current_branch, 1);
}

void show_stash_list(){
    FILE* file = fopen(get_stash_info_addres(), "rb");
    fseek(file, 0, SEEK_END);
    int ptr = ftell(file);
    Commit sts;
    int cnt = 0;
    while(ptr > 0){
        ptr -= sizeof(sts);
        fseek(file, ptr, SEEK_SET);
        fread(&sts, sizeof(sts), 1, file);
        if(sts.date < 0) continue;
        
        sts.date = cnt++; 
        print_stash(sts);
    }
    fclose(file);
}

void stash_show_diff(int num){
    int index;
    Commit sts = get_stash(num, &index);
    show_commit_diff(sts.commit_id, sts.creator);
}

int stash_drop(int num){
    int index;
    Commit sts = get_stash(num, &index);
    sts.date = -1;
    upate_middle_file(&sts, sizeof(sts), index, get_stash_info_addres());
    return 1;
}

int stash_pop(int num){
    if(check_diff_in_project()){
        print_fail("project have changes in files that not been committed, you can seen them with \"gitil status -p\"!");
        return 0;
    }
    int index;
    Commit sts = get_stash(num, &index);
    if(merge_branch(sts.branch_name, get_current_commit())){
        sts.date = -1;
        upate_middle_file(&sts, sizeof(sts), index, get_stash_info_addres());
        return 1;
    }
    return 0;
}

void stash_clear(){
    int len = get_file_len(get_stash_info_addres(), sizeof(Commit));
    while(len--) stash_drop(0);
}

int stash_branch(char* branch_name, int num){
    if(check_diff_in_project()){
        print_fail("project have changes in files that not been committed, you can seen them with \"gitil status -p\"!");
        return 0;
    }
    int index;
    Commit sts = get_stash(num, &sts, &index);
    checkout_commit(sts.creator, 1);
    create_branch(branch_name, 0);
    // after writing revert...
}