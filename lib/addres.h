/* Project Addres */
char *get_root_addres();
char *get_current_addres();
char* get_gitil_addres();
char *get_rel_addres_from(char* addres, char *from);
char* get_rel_addres(char* addres);

/* Config Addres*/
char *get_local_config_addres();
char *get_global_config_addres();
char *get_local_alias_addres();
char *get_global_alias_addres();
char *get_global_folder_addres();
char *get_shortcut_commit_addres();

/* Branch Addres */
char *get_branch_info_addres();
char *get_main_branch_folder_addres();
char *get_branch_folder_addres(char* branch_name);
char *get_stage_info_addres(char* branch_name);
char *get_unstage_info_addres(char* branch_name);
char *get_commit_info_addres(char* branch_name);
char *get_stage_changes_folder_addres(char* branch_name);
char *get_stage_changes_file_addres(char* branch_name, char *file_name);
char *get_mem_stage_info_addres(char* branch_name);
char *get_current_branch_folder_addres();
char *get_current_stage_info_addres();
char *get_current_unstage_info_addres();
char *get_current_commit_info_addres();
char *get_current_stage_changes_folder_addres();
char *get_current_stage_changes_file_addres(char *file_name);
char *get_current_mem_stage_info_addres();

/* Commit Addres */
char *get_commits_info_addres();
char *get_commits_folder_addres(char* branch_name);
char *get_commit_folder_addres(char* commit_id);
char *get_commit_status_file_addres(char* commit_id);
char *get_commit_saved_file_addres(char* commit_addres, char* file_addres);