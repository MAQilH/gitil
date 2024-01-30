#include "file_model.h"
#include <stdio.h>
#include "../lib/print.h"
#include "../lib/stril.h"
#include <string.h>

char* get_state_string(State sts){
    if(sts == Modified) return "Modified";
    if(sts == Delete) return "Delete";
    if(sts == Unchange) return "Unchange";
    if(sts == Create) return "Create";
    if(sts == NotFound) return "NotFound";
    if(sts == Access) return "Access";
}

void print_file(File fl){
    printf(
        "name: %s\n"
        "addres: %s\n"
        "state: %s\n",
        fl.name, fl.addres, get_state_string(fl.state)
    );
    print_fail("- - - - -");
}

void print_file_file(char* file_addres){
    FILE* file = fopen(file_addres, "rb");
    File tmp;
    while(fread(&tmp, sizeof(tmp), 1, file)){
        print_file(tmp);
    }
    fclose(file);
}

File blanck_file(){
    File fl;
    strcpy(fl.name, "");
    strcpy(fl.addres, "");
    fl.state = NotFound;
    return fl;
}

int is_blanck(File* file){
    return strlen(file->addres) == 0;
}