#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME (260) 

int file_cmp(char* file_addres1, char* file_addres2){
    FILE* file1 = fopen(file_addres1, "r");
    FILE* file2 = fopen(file_addres2, "r");
    char c1, c2;
    while(1){
        c1 = fgetc(file1);
        c2 = fgetc(file2);
        if(c1 != c2){
            fclose(file1);
            fclose(file2);
            return 0;
        }
        if(c1 == EOF){
            fclose(file1);
            fclose(file2);
            return 1;
        }
    }
}

int main(){
    // int a;
    // FILE *file = fopen("branch_info.dat", "rb");
    // Branch brn;
    // fread(&brn, sizeof(brn), 1, file);
    // fclose(file);
    // printf("%s %s %s %d %d", brn.creator, brn.name, brn.parrent_branch, brn.date, brn.head_commit_id);
    
    printf("%d", file_cmp("tx1.txt", "tx2.txt"));
   
    return 0;
}