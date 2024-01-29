#include "../glob.h"
typedef struct Commit{
    char message[MAX_MESSAGE];
    char creator[MAX_NAME];
    char branch_name[MAX_NAME];
    char commit_id[MAX_NAME];
    int date;
} Commit;

void print_commit(Commit cmt);
void print_commit_file(char* file_addres);