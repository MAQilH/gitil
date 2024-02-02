#include "../lib/lib.h"
#include "../model/file_list_model.h"
#include "diff.h"
#include "commit.h"

int merge_validate(char *commit_id1, char *commit_id2){
    FileList commit_status1 = get_commit_status_file(commit_id1);
    FileList commit_status2 = get_commit_status_file(commit_id2);
    int have = 0;
    for(int i = 0; i < commit_status1.cnt; i++){
        if(find_index_in_file_list(&commit_status2, commit_status1.lst[i].addres) != -1){
            if(file_cmp(exist_in_commit(commit_id1, commit_status1.lst[i].addres), exist_in_commit(commit_id2, commit_status1.lst[i].addres)))
                continue;
            print_warn(cat_string("<<<<<<<", cat_string(get_rel_addres(commit_status1.lst[i].addres), ">>>>>>>")));
            have |= print_diff_file(
                exist_in_commit(commit_id1, commit_status1.lst[i].addres), 1, INF,
                exist_in_commit(commit_id2, commit_status1.lst[i].addres), 1, INF
            );
        }
    }
    return !have;
}

int merge_branch(char* branch1, char* branch2){
    char *commit_id1 = get_branch_head_commit(branch1);
    char *commit_id2 = get_branch_head_commit(branch2);
    if(merge_validate(commit_id1, commit_id2)){

    }
}
