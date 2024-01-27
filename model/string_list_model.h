#include "../glob.h"
typedef struct StringList{
    int cnt;
    char **lst;
} StringList;
StringList get_string_list(char* str, char* del);
