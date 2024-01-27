#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include "../lib/print.h"
#include "../lib/file_manage.h"

int init(int argc, char *argv[]){
    if(get_root_project_addres() != NULL){
        print_fail("fail: gitil already created!");
        return 0;
    }
    char* addres = get_curren_addres();
    strcat(addres, "/.gitil");
    create_hidden_folder(addres);
    print_success("gitil created!");
    return 1;
}