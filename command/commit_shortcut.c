#include "../model/alias_model.h"
#include "../lib/lib.h"

int set_commit_shortcut(char* msg, char* shortcut){
    print_error(msg);
    print_error(shortcut);
    Alias als;
    strcpy(als.from, shortcut);
    strcpy(als.to, msg);
    FILE *file = fopen(get_shortcut_commit_addres(), "ab");
    fwrite(&als, sizeof(als), 1, file);
    fclose(file);
    return 1;
}

char* get_commit_shortcut(char* shortcut){
    Alias als;
    FILE *file = fopen(get_shortcut_commit_addres(), "rb");
    while(fread(&als, sizeof(als), 1, file)){
        if(!strcmp(als.from, shortcut)){
            return get_string_ref(als.to);
        }
    }
    return NULL;
}

int replace_commit_shortcut(char* msg, char* shortcut){
    Alias als;
    FILE *file = fopen(get_shortcut_commit_addres(), "rb+");
    int index = 0, have = 0;
    while(fread(&als, sizeof(als), 1, file)){
        if(!strcmp(als.from, shortcut)){
            have = 1;
            break;
        }
        index++;
    }
    if(have == 0) return 0;
    strcpy(als.to, msg);
    if(strlen(msg) == 0){
        strcpy(als.from, msg);
    }
    upate_middle_file(&als, sizeof(als), index, get_shortcut_commit_addres());
    return 1;
}

int remove_commit_shortcut(char* shortcut){
    return replace_commit_shortcut("", shortcut);
}

int shortcut(int argc, char* argv[]){
    if(!strcmp(argv[1], "set")){
        if(set_commit_shortcut(argv[3], argv[5])){
            print_success("shortcut set!");
        } else{
            print_fail("gitil in updating please wait...");
        }
    } else if(!strcmp(argv[1], "replace")){
        if(replace_commit_shortcut(argv[3], argv[5])){
            print_success("shortcut replace!");
        } else{
            print_fail("fail: shortcut not founded!");
        }
    } else if(!strcmp(argv[1], "remove")){
        if(remove_commit_shortcut(argv[3])){
            print_success("shortcut removed!");
        } else{
            print_fail("fail: shortcut not founded!");
        }
    }
}