#include "diff.h"
#include "commit.h"
#include "branch.h"
#include "add.h"
#include "status.h"
#include "../lib/lib.h"
#include "../model/commit_model.h"
#include "../model/file_list_model.h"

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

    FileList *commit_status1 = get_commit_status_file(commit_id1);
    FileList *commit_status2 = get_commit_status_file(commit_id2);
    int have = 0;
    for(int i = 0; i < commit_status1->cnt; i++){
        if(find_index_in_file_list(commit_status2, commit_status1->lst[i].addres) != -1){
            if(
                commit_status1->lst[i].state == Delete || 
                find_in_file_list(commit_status2, commit_status1->lst[i].addres) == Delete ||
                file_cmp(exist_in_commit(commit_id1, commit_status1->lst[i].addres), exist_in_commit(commit_id2, commit_status1->lst[i].addres))    
            )
                continue;
            print_warn(cat_string("<<<<<<<", cat_string(get_rel_addres(commit_status1->lst[i].addres), ">>>>>>>")));
            have |= print_diff_file(
                exist_in_commit(commit_id1, commit_status1->lst[i].addres), 1, INF,
                exist_in_commit(commit_id2, commit_status1->lst[i].addres), 1, INF
            );
        }
    }
    if(have){
        print_fail("you must resolve conflicte and then merge this two branch!");
        return 0;
    }
    return 1;
}

int merge_branch(char* branch1, char* branch2){
    if(merge_validate(branch1, branch2)){
        char *commit_id1 = get_branch_head_commit(branch1);
        char *commit_id2 = get_branch_head_commit(branch2);
        FileList* commit1_file_status = get_file_list(get_commit_status_file_addres(commit_id1));
        FileList* commit2_file_status = get_file_list(get_commit_status_file_addres(commit_id2));
        for(int i = 0; i < commit2_file_status->cnt; i++){
            if(commit2_file_status->lst[i].state == Delete) continue;
            int index = find_index_in_file_list(commit1_file_status, commit2_file_status->lst[i].addres);
            if(index == -1 || find_in_file_list(commit1_file_status, commit2_file_status->lst[i].addres) == Delete){
                file_copy(
                    get_commit_saved_file_addres(commit_id2, commit2_file_status->lst[i].addres),
                    commit2_file_status->lst[i].addres
                );
            }
        }
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
