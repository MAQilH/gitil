#include "../glob.h"
// #include "state_model.h"
typedef enum{
    Modified,
    Create,
    Delete,
    Unchange,
    NotFound
} State;
typedef struct{
    char name[MAX_NAME];
    char addres[MAX_ADDRES];
    State state;
} File;

State get_file_state_with_commit(char *commit_id, char* file_addres);
void print_file_file(char* file_addres);
void print_file(File fl);