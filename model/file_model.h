#include "../glob.h"
// #include "state_model.h"
typedef enum{
    Modified,
    Create,
    Delete,
    Unchange,
    NotFound,
    Access
} State;
typedef struct{
    char name[MAX_NAME];
    char addres[MAX_ADDRES];
    State state;
} File;

State get_file_state_with_commit(char *commit_id, char* file_addres);
void print_file_file(char* file_addres);
void print_file(File fl);
File blanck_file();
int is_blanck(File* fl);
void remove_file_in_stage_change(File* fl);
char* get_state_string(State sts);
char* get_name_file_in_stage_change(File* fl);
