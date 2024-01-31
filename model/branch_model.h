#include "../glob.h"
typedef struct Branch{
    char name[MAX_NAME];
    char creator[MAX_NAME];
    char parrent_branch[MAX_NAME]; 
    char parent_commit_id[MAX_NAME];
    char head_commit_id[MAX_NAME];
    int date;
} Branch;

Branch get_branch(char* branch_name);
void print_branch(Branch brn);
void print_branch_file(char* file_addres);