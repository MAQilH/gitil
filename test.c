#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
// #include "model/model.h"

int static_error_check_hook(char* file_addres){
    // if(!check_type_support(5, file_addres)) return 0;
    // char *file_type = get_file_type(file_addres);
    char command[1000];
    char *error_addres = "error.txt";
    
    // if(!strcmp(file_type, "cpp")){
    //     snprintf(command, sizeof(command), "g++ -o 0 %s 2> %s", file_addres, error_addres);
    // }else {
    //     snprintf(command, sizeof(command), "gcc -o 0 %s 2> %s", file_addres, error_addres);
    // }
    snprintf(command, sizeof(command), "g++ -o 0 %s 2> %s", file_addres, error_addres);

    int result = system(command);
    // print_fail(file_addres);
    // print_success(error_addres);
    if(result == -1){
        // print_error("failed to execute compiler command.\n");
        remove(error_addres);
        return -1;
    }

    FILE* error_file = fopen(error_addres, "r");
    if(error_file == NULL){
        // print_error("failed to open error file.\n");
        remove(error_addres);
        return -1;
    }

    fseek(error_file, 0, SEEK_END);
    int file_size = ftell(error_file);
    fclose(error_file);
    remove(error_addres);
    return file_size > 0? 0: 1;
}

void time_limit_hook(char* file_addres){
    char command[512];
    snprintf(command, sizeof(command),
             "ffprobe -v error -show_entries format=duration -of default=noprint_wrappers=1:nokey=1 \"%s\"",
             file_addres);

    FILE *pipe = popen(command, "r");
    if (!pipe) {
        perror("popen failed");
        return;
    }

    char buffer[1000];
    if (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        printf("Duration: %s seconds\n", buffer);
    } else {
        printf("Failed to get duration for file %s\n", file_addres);
    }

    // Close the pipe
    pclose(pipe);
}

int main(){
    printf("%d\n", static_error_check_hook("C:\\Users\\moham\\C++\\gitil\\kGAws1w92buOlOgkC7fe.cpp"));
    return 0;
}