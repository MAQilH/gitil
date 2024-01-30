#include "file_model.h"
typedef struct FileList{
    int cnt;
    File lst[200];
} FileList;
void merge_file_list(FileList *dest, FileList src);
void print_file_list(FileList flst);
void print_file_list_file(char* file_addres);
State find_in_file_list(FileList *flst, char* query);
State find_in_file_list_with_addres(char* file_addres, char *query);
void add_to_stage(FileList *flst);
int find_index_in_file_list(FileList *flst, char* query);
int find_index_in_file_list_with_addres(char *file_addres, char* query);
FileList get_commit_status_file(char *commit_id);
void get_file_status(FileList *flst, char* folder_addres, int dep);
FileList get_clean_file_list(FileList* flst);
void add_to_undo_file(FileList *flst);