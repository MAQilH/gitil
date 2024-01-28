typedef enum{
    Modified,
    Create,
    Delete,
    Unchange
} State;

State get_file_state_with_commit(char *commit_id, char* file_addres);