#include "../glob.h"
typedef struct Commit{
    char message[MAX_NAME];
    char creator[MAX_NAME];
    int commit_id;
    int date;
} Commit;