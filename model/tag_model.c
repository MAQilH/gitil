#include "tag_model.h"
#include "../lib/stril.h"
#include <stdio.h>

void print_tag(Tag tag){
    printf(
        "name: %s\n"
        "commit: %s\n"
        "creator: %s\n"
        "message: %s\n"
        "date: %s\n\n",
        tag.tag_name, tag.commit_id, tag.creator, tag.message, get_date(tag.date)
    );
}

