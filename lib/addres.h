/* Proect Addres */
char *get_root_addres();
char *get_current_addres();
char* get_gitil_addres();

/* Config Addres*/
char *get_local_config_addres();
char *get_global_config_addres();
char *get_local_alias_addres();
char *get_global_alias_addres();
char *get_global_folder_addres();

/* Branch Addres */
char *get_branch_info_addres();
char *get_main_branch_folder_addres();
char *get_branch_folder_addres(char* branch_name);
char *get_stage_info_addres(char* branch_name);
char *get_unstage_info_addres(char* branch_name);
char *get_commit_info_addres(char* branch_name);
char *get_current_branch_folder_addres();
char *get_current_stage_info_addres();
char *get_current_unstage_info_addres();
char *get_current_commit_info_addres();