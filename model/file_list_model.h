#include "file_model.h"
typedef struct FileList{
    int cnt;
    File lst[MAX_NAME];
} FileList;
void merge_file_list(FileList *dest, FileList src);
void print_file_list(FileList flst);
void print_file_list_file(char* file_addres);