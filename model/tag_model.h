#include "../glob.h"
typedef struct Tag{
    char tag_name[MAX_MESSAGE];
    char commit_id[MAX_NAME];
    char creator[MAX_NAME];
    char message[MAX_MESSAGE];
    int date;
} Tag;

void print_tag(Tag tag);