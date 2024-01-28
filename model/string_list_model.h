#include "../glob.h"
typedef struct StringList{
    int cnt;
    char **lst;
} StringList;
StringList get_string_list(char* str, char* del);
void print_string_list(StringList stl);
void print_string_list_file(char* file_addres);
