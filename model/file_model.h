#include "../glob.h"
// #include "state_model.h"
typedef enum{
    Modified,
    Create,
    Delete,
    Unchange
} State;
typedef struct{
    char name[MAX_NAME];
    char addres[MAX_NAME];
    State state;
} File;

State get_file_state_with_commit(char *commit_id, char* file_addres);