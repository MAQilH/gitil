#include "../glob.h"
#include "file_model.h"
typedef struct FileList{
    int __cur_len;
    int cnt;
    File *lst;
} FileList;
void print_file_list(FileList *flst);
void print_file_list_file(char* file_addres);
State find_in_file_list(FileList *flst, char* query);
State find_in_file_list_with_addres(char* file_addres, char *query);
void add_to_stage(FileList *flst);
int find_index_in_file_list(FileList *flst, char* query);
int find_index_in_file_list_with_addres(char *file_addres, char* query);
FileList* get_commit_status_file(char *commit_id);
void get_file_status(FileList *flst, char* folder_addres, int dep);
FileList* get_clean_file_list(FileList* flst);
void add_to_undo_file(FileList *flst);
FileList* get_file_list(char* addres);
void set_file_list(char* addres, FileList* flst);
void file_list_push_back(FileList* flst, File* fl);
FileList* create_file_list(int num);
int add_file_rel_addres_to_file_list(FileList *flst, char* addres);
void get_file_status_with_commit(char* commit_id, FileList *flst, char* folder_addres, int dep);