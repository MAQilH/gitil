#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include "lib.h"

/* Project Addres */

char* get_current_addres(){
    char cwd[260];
    getcwd(cwd, sizeof(cwd));
    return cat_string(get_string_ref(cwd), "\\");
}

char* get_root_addres(){
    char cwd[260];
    char *saved = get_current_addres();
    while(getcwd(cwd, sizeof(cwd)) != NULL && strcmp(cwd, "C:\\")){
        if(check_in_root()){
            chdir(saved);
            return cat_string(get_string_ref(cwd), "\\");
        }
        chdir("..");
    }
    chdir(saved);
    return NULL;
}

char* get_gitil_addres(){
    return cat_string(get_root_addres(), ".gitil\\");
}

char *get_rel_addres(char* addres){
    return addres + strlen(get_current_addres());
}

/* Config Addres */

char* get_local_config_addres(){
    return cat_string(get_gitil_addres(), "config_info.dat");
}

char* get_global_config_addres(){
    return "C:\\.gitil\\config_info.dat";
}

char* get_local_alias_addres(){
    return cat_string(get_gitil_addres(), "alias_info.dat");
}

char* get_global_alias_addres(){
    return "C:\\.gitil\\alias_info.dat";
}

char* get_global_folder_addres(){
    return "C:\\.gitil\\";
}

/* Branch Addres */

char* get_branch_info_addres(){
    return cat_string(get_gitil_addres(), "branch_info.dat");
}

char *get_main_branch_folder_addres(){ 
    return cat_string(get_gitil_addres(), "branch\\");
}

char *get_branch_folder_addres(char* branch_name){ // \.gitil\branch\branch123
    return cat_string(get_main_branch_folder_addres(), cat_string(branch_name, "\\"));
}

char *get_stage_info_addres(char* branch_name){ // \.gitil\branch\branch123\stage_info.dat
    return cat_string(get_branch_folder_addres(branch_name), "stage_info.dat");
}

char *get_unstage_info_addres(char* branch_name){ // \.gitil\branch\branch123\unstage_info.dat
    return cat_string(get_branch_folder_addres(branch_name), "unstage_info.dat");
}

char *get_commit_info_addres(char* branch_name){  // \.gitil\branch\branch123\commit_info.dat
    return cat_string(get_branch_folder_addres(branch_name), "commit_info.dat");
}

char *get_stage_changes_folder_addres(char* branch_name){
    return cat_string(get_branch_folder_addres(branch_name), "stage_change\\"); 
}

char *get_stage_changes_file_addres(char* branch_name, char *file_name){
    return cat_string(get_stage_changes_folder_addres(branch_name), file_name);
}

char *get_current_branch_folder_addres(){
    return get_branch_folder_addres(get_HEAD());
}

char *get_current_stage_info_addres(){
    return get_stage_info_addres(get_HEAD());
}

char *get_current_unstage_info_addres(){
    return get_unstage_info_addres(get_HEAD());
}

char *get_current_commit_info_addres(){
    return get_commit_info_addres(get_HEAD());
}

char *get_current_stage_changes_folder_addres(){
    return get_stage_changes_folder_addres(get_HEAD());
}

char *get_current_stage_changes_file_addres(char *file_name){
    return get_stage_changes_file_addres(get_HEAD(), file_name);
}


/* Commit */

char *get_commits_folder_addres(char* branch_name){
    return strcat(get_branch_folder_addres(branch_name), "commits\\");
}

char *get_commit_folder_addres(char* commit_id){
    return cat_string(
        get_commits_folder_addres(get_commit_branch(commit_id)),
        cat_string(commit_id, "\\")
    );
}

char *get_commit_status_file_addres(char* commit_id){
    return cat_string(get_commit_folder_addres(commit_id), "file_status.dat");
}
