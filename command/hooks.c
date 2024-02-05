#include "hooks.h"
#include "../lib/lib.h"
#include "../model/string_list_model.h"
#include "diff.h"

char** get_hook_format(int num, int *num_format){
    char** res = (char**)calloc(10, sizeof(char*));
    for(int i = 0; i < 7; i++) res[i] = get_string(10);
    if(num == 0 || num == 1 || num == 3 || num == 4 || num == 5 || num == 7){
        strcpy(res[0], "cpp");
        strcpy(res[1], "c");
        *num_format += 2;
    }
    if(num == 0 || num == 1 || num == 3 || num == 7){
        strcpy(res[*num_format], "txt");
        *num_format += 1;
    } 
    if(num == 2){
        strcpy(res[0], "cpp");
        strcpy(res[1], "c");
        strcpy(res[2], "txt");
        strcpy(res[3], "mp4");
        strcpy(res[4], "mp3");
        strcpy(res[5], "wav");
        strcpy(res[6], "py");
        strcpy(res[6], "exe");
        *num_format = 8;
    }
    if(num == 6){
        strcpy(res[0], "*");
        *num_format = 1;
    }
    if(num == 8){
        strcpy(res[0], "mp3");
        strcpy(res[1], "wav");
        strcpy(res[2], "mp4");
        *num_format = 3;
    }
    return res;
}

int check_type_support(int hook_num, char* file_addres){
    int num_type_sup = 0;
    char** type_sup = get_hook_format(hook_num, &num_type_sup);
    int have = 0;
    char* file_type = get_file_type(file_addres);
    for(int i = 0; i < num_type_sup; i++){
        if(!strcmp(file_type, type_sup[i])){
            return 1;
        }
    }
    return 0;
}

HookState todo_check_hook(char* file_addres){
    if(!check_type_support(0, file_addres)) return SKIPPED;
    char* file_type = get_file_type(file_addres);
    FILE *file = fopen(file_addres, "r");
    char *s = get_string(MAX_STR);
    while(fgets(s, MAX_STR, file)){
        StringList strl = get_string_list(s, " ");
        int bg = 0;
        if(strcmp(file_type, "txt")){
            while(bg < strl.cnt && strcmp(strl.lst[bg], "//")) bg++;
        }
        for(int i = bg; i < strl.cnt; i++){
            if(!strcmp(strl.lst[i], "TODO")){
                fclose(file);
                return FAILED;
            }
        }
    }
    fclose(file);
    return PASSED;
}

HookState eof_blank_space_hook(char* file_addres){
    if(!check_type_support(1, file_addres)) return SKIPPED;
    char* file_type = get_file_type(file_addres);
    FILE *file = fopen(file_addres, "r");
    char *s = get_string(MAX_STR);
    char *last_line = get_string(MAX_STR);
    int at_least = 0;
    while(fgets(s, MAX_STR, file)){
        at_least = 1;
        strcpy(last_line, s);
    } 
    fclose(file);
    if(!at_least && strlen(last_line) == 0) return PASSED;
    if(check_nullspace_string(last_line) || (strlen(last_line) && last_line[strlen(last_line)-1] == '\n')){
        return FAILED;
    }
    return PASSED;
}

HookState format_check_hook(char* file_addres){
    if(!check_type_support(2, file_addres)) return FAILED;
    return PASSED;
}

HookState balance_braces_hook(char* file_addres){
    if(!check_type_support(3, file_addres)) return SKIPPED;
    FILE *file = fopen(file_addres, "r");
    char *s = get_string(MAX_STR);
    char *stk = get_string(MAX_STR*100);
    int ptr = 0;
    while(fgets(s, MAX_STR, file)){
        int str_len = strlen(s);
        for(int i = 0; i < str_len; i++){
            char rev = '\0';
            if(s[i] == '(' || s[i] == '[' || s[i] == '{'){
                stk[ptr++] = s[i];
            } else if(s[i] == ')'){
                rev = '(';
            } else if(s[i] == '}'){
                rev = '{';
            } else if(s[i] == ']'){
                rev = '[';
            }
            if(rev != '\0'){
                if(ptr && stk[ptr-1] == rev){
                    ptr--;
                    continue;
                }
                fclose(file);
                return FAILED;
            }
        }
    } 
    if(ptr) return FAILED;
    return PASSED;
}

HookState indentation_check_hook(char* file_addres){
    if(!check_type_support(4, file_addres)) return SKIPPED;
    // TODO
    return PASSED;
}

HookState static_error_check_hook(char* file_addres){
    if(!check_type_support(5, file_addres)) return SKIPPED;
    char *file_type = get_file_type(file_addres);
    char command[MAX_STR];
    char *error_addres = get_error_addres();
    
    if(!strcmp(file_type, "cpp")){
        snprintf(command, sizeof(command), "g++ -o null %s 2> %s", file_addres, error_addres);
    }else {
        snprintf(command, sizeof(command), "gcc -o null %s 2> %s", file_addres, error_addres);
    }

    int result = system(command);
    if(result == -1){
        print_error("failed to execute compiler command.\n");
        remove(error_addres);
        return -1;
    }

    FILE* error_file = fopen(error_addres, "r");
    if(error_file == NULL){
        print_error("failed to open error file.\n");
        remove(error_addres);
        return -1;
    }

    fseek(error_file, 0, SEEK_END);
    int file_size = ftell(error_file);
    fclose(error_file);
    remove(error_addres);
    return file_size > 0? FAILED: PASSED;
}

HookState file_size_check_hook(char* file_addres){
    FILE *file = fopen(file_addres, "rb");
    if(file == NULL) return SKIPPED;
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);
    return size > (5ll*(1ll<<20ll))? FAILED: PASSED;
}

HookState character_limit_hook(char* file_addres){
    if(!check_type_support(7, file_addres)) return SKIPPED;
    FILE *file = fopen(file_addres, "r");
    int cnt = 20000;
    while(fgetc(file) != EOF){
        cnt--;
        if(cnt < 0){
            fclose(file);
            return FAILED;
        }
    }
    return PASSED;
}

HookState time_limit_hook(char* file_addres){
    if(!check_type_support(8, file_addres)) return SKIPPED;
    char command[MAX_STR];
    snprintf(command, sizeof(command),
            "ffprobe -v error -show_entries format=duration -of default=noprint_wrappers=1:nokey=1 \"%s\"",
            file_addres);

    FILE *pipe = popen(command, "r");
    if (!pipe) {
        perror("popen failed!");
        return PASSED;
    }

    char buffer[MAX_STR];
    if (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        pclose(pipe);
        int sec = stoi(buffer);
        return sec <= 10*60? PASSED: FAILED;
    } else {
        pclose(pipe);
        return PASSED;
    }
}

int resolve_eof_blank_space(char* file_addres){
    if(!check_type_support(1, file_addres)) return 1;
    FILE* file = fopen(file_addres, "r");
    char *s = get_string(MAX_STR);
    int last_valid_line = -1;
    int line_number = -1;
    while(fgets(s, MAX_STR, file)){
        line_number++;
        if(check_nullspace_string(s)) continue;
        last_valid_line = line_number;
    }
    fclose(file);
    FILE *tmp_file = fopen(get_error_addres(), "w");
    file = fopen(file_addres, "r");
    line_number = -1;
    while(fgets(s, MAX_STR, file)){
        if(line_number == last_valid_line){
            break;
        }
        line_number++;
        if(line_number == last_valid_line && strlen(s) && s[strlen(s)-1] == '\n'){
            s[strlen(s)-1] = '\0';
        }
        fprintf(tmp_file, "%s", s);
    }
    fclose(tmp_file);
    fclose(file);
    file_copy(get_error_addres(), file_addres);
    remove(get_error_addres());
    return 1;
}

char get_inv_braces(char c){
    if(c == '['){
        return ']';
    }
    if(c == ']'){
        return '[';
    }
    if(c == '{'){
        return '}';
    }
    if(c == '}'){
        return '{';
    }
    if(c == '('){
        return ')';
    }
    if(c == ')'){
        return '(';
    }
    return '\0';
}

int resolve_balance_braces(char* file_addres){
    if(!check_type_support(3, file_addres)) return SKIPPED;
    FILE *file = fopen(file_addres, "r");
    FILE *tmp_file = fopen(get_error_addres(), "w");
    char *s = get_string(MAX_STR);
    char *stk = get_string(MAX_STR*100);
    int ptr = 0;
    while(fgets(s, MAX_STR, file)){
        int str_len = strlen(s);
        for(int i = 0; i < str_len; i++){
            char rev = '\0';
            if(s[i] == '(' || s[i] == '[' || s[i] == '{'){
                stk[ptr++] = s[i];
                fprintf(tmp_file, "%c", s[i]);
                continue;
            }
            rev = get_inv_braces(s[i]);
            if(rev != '\0'){
                if(ptr && stk[ptr-1] == rev){
                    fprintf(tmp_file, "%c", s[i]);
                    ptr--;
                    continue;
                }
                fprintf(tmp_file, "%c%c", rev, s[i]);
                continue;
            }
            fprintf(tmp_file, "%c", s[i]);
        }
    } 
    while(ptr){
        fprintf(tmp_file, "%c", get_inv_braces(stk[--ptr]));
    }
    fclose(file);
    fclose(tmp_file);
    file_copy(get_error_addres(), file_addres);
    remove(get_error_addres());
    return 1;
}

int resove_indentation_check(char* file_addres){
    if(!check_type_support(4, file_addres)) return SKIPPED;
    // TODO
    return 1;
}