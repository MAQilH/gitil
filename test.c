#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME (260) 

typedef struct Branch{
    char name[MAX_NAME];
    char creator[MAX_NAME];
    char parrent_branch[MAX_NAME]; 
    int parent_commit_id;
    int date;
    int head_commit_id;
} Branch;

int main(){
    int a;
    FILE *file = fopen("branch_info.dat", "rb");
    Branch brn;
    fread(&brn, sizeof(brn), 1, file);
    fclose(file);
    printf("%s %s %s %d %d", brn.creator, brn.name, brn.parrent_branch, brn.date, brn.head_commit_id);
    return 0;
}