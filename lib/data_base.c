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
    FILE *branch_info_file = fopen(get_branch_info_addres(), "rb");
    Branch brn;
    while(fread(&brn, sizeof(brn), 1, branch_info_file)){
        FILE *commit_info_file = fopen(get_commit_info_addres(brn.name), "rb");
        Commit cmt;
        while(fread(&cmt, sizeof(cmt), 1, commit_info_file)){
            if(!strcmp(cmt.commit_id, commit_id)){
                fclose(commit_info_file);
                fclose(branch_info_file);
                return get_string_ref(brn.name);
            }
        } 
        fclose(commit_info_file);
    }
    fclose(branch_info_file);
}

char *get_branch_head_commit(char* branch_name){
    return get_string_ref(get_branch(branch_name).head_commit_id);
}

char* get_cuurent_HEAD_commit(){
    return get_branch_head_commit(get_HEAD());
}