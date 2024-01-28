#include "../lib/lib.h"
// #include "../model/state_model.h"
#include "../model/commit_model.h"
#include "../model/file_list_model.h"

char* exist_in_commit(char *commit_id, char* file_addres){
    FILE *file_status = fopen(get_commit_file_status_addres(commit_id), "rb");
    FileList flst;
    fread(&flst, sizeof(flst), 1, file_status);
    fclose(file_status);
    for(int i = 0; i < flst.cnt; i++){
        if(!strcmp(file_addres, flst.lst[i].addres)){
            return cat_string(
                cat_string(itos(i), "."),
                get_file_type(file_addres)
            );
        }
    }
    return NULL;
}

State get_file_state_with_commit(char *commit_id, char* file_addres){
    char* addres_in_commit = exist_in_commit(commit_id, file_addres);
    if(!exist_file(file_addres)){
        if(addres_in_commit == NULL) return Unchange;
        return Delete;
    }
    if(addres_in_commit == NULL) return Create;
    if(file_cmp(addres_in_commit, file_addres)) return Unchange;
    return Modified;
}

char* create_random_commit_id(){
    int LEN = 20;
    char *res = (char*)calloc(LEN, 1);
    for(int i = 0; i < LEN; i++){
        res[i] = (char)('a' + rand()%26);
    }
    return res;
}

