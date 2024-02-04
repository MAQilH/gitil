#include "tag_model.h"
#include "../lib/stril.h"
#include <stdio.h>

void print_tag(Tag tag){
    printf(
        "\e[1mName:\e[m %s\n"
        "\e[1mID:\e[m \033[1;33m%s\033[0m\n"
        "\e[1mCreator:\e[m %s\n"
        "\e[1mMessage:\e[m %s\n"
        "\e[1mDate:\e[m %s\n\n",
        tag.tag_name, tag.commit_id, tag.creator, tag.message, get_date(tag.date)
    );
}

