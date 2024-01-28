#include "../glob.h"
typedef struct Commit{
    char message[MAX_NAME];
    char creator[MAX_NAME];
    char branch_name[MAX_NAME];
    char commit_id[MAX_NAME];
    int date;
} Commit;