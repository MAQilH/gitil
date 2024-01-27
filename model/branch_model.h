#include "../glob.h"
typedef struct Branch{
    char name[MAX_NAME];
    char creator[MAX_NAME];
    char parrent_branch[MAX_NAME]; 
    int parent_commit_id;
    int date;
    int head_commit_id;
} Branch;