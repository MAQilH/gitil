#include "lib.h"
#include "../model/config_model.h"
#include "../model/branch_model.h"
#include "../model/commit_model.h"

char* get_creator(){
    Config loc = get_config(get_local_config_addres()), glo = get_config(get_global_config_addres());

    if(strlen(loc.name) == 0){
        return get_string_ref(glo.name);
    }
    return get_string_ref(loc.name);
}

char* get_HEAD(){
    Config loc = get_config(get_local_config_addres());
    return get_string_ref(loc.head);
}

char* get_commit_branch(char *commit_id){
    FILE *commits_file = fopen(get_commits_info_addres(), "rb");
    Commit cmt;
    while(fread(&cmt, sizeof(cmt), 1, commits_file)){
        if(!strcmp(cmt.commit_id, commit_id)){
            break;
        }
    }
    fclose(commits_file);
    return get_string_ref(cmt.branch_name);
}

char *get_branch_head_commit(char* branch_name){
    return get_string_ref(get_branch(branch_name).head_commit_id);
}

char* get_cuurent_HEAD_commit(){
    return get_branch_head_commit(get_HEAD());
}

char* get_current_commit(){
    Config loc = get_config(get_local_config_addres());
    return get_string_ref(loc.current_commit);
}

void set_HEAD_branch(char* branch_name){
    Config loc = get_config(get_local_config_addres());
    strcpy(loc.current_commit, get_branch(branch_name).head_commit_id);
    strcpy(loc.head, branch_name);
    set_config(get_local_config_addres(), &loc);
}

void set_current_commit(char *commit_id){
    Config loc = get_config(get_local_config_addres());
    strcpy(loc.current_commit, commit_id);
    set_config(get_local_config_addres(), &loc);
}

int get_rand(){
    Config loc = get_config(get_local_config_addres());
    srand(loc.seed);
    int res = rand();
    res = abs(res);
    loc.seed = res;
    set_config(get_local_config_addres(), &loc);
    return res;
}