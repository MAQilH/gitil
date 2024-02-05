#include "../lib/lib.h"
#include "../model/config_model.h"
#include "../model/file_list_model.h"
#include "hooks.h"
#include "stage.h"

char* get_hook_name(int num){
    switch(num){
        case 0:
            return "todo-check";
        case 1:
            return "eof-blank-space";
        case 2:
            return "format-check";
        case 3: 
            return "balance-braces";
        case 4:
            return "indentation-check";
        case 5:
            return "static-error-check";
        case 6:
            return "file-size-check"; 
        case 7:
            return "character-limit";
        case 8:
            return "time-limit";
        default:
            return NULL;
    }
}

int get_hook_num(char *hook_name){
    for(int i = 0; i < HOOK_NUMBER; i++){
        if(!strcmp(hook_name, get_hook_name(i))){
            return i;
        }
    }
    return -1;
}

int get_hook(){
    Config conf = get_config(get_local_config_addres());
    return conf.hook;
}

void set_hook(int hook){
    Config conf = get_config(get_local_config_addres());
    conf.hook = hook;
    set_config(get_local_config_addres(), &conf);
}

int show_hooks_list(){
    for(int i = 0; i < HOOK_NUMBER; i++){
        printf("    **%s\n", get_hook_name(i));
    }
    return 1;
}

int show_applied_hooks(){
    int hook = get_hook();
    for(int i = 0; i < HOOK_NUMBER; i++){
        if(hook>>i&1){
            printf("    **%s\n", get_hook_name(i));
        }
    }
    return 1;
}

int add_hook(char* hook_name){
    int hook = get_hook();
    hook |= 1<<get_hook_num(hook_name);
    set_hook(hook);
}

int remove_hook(char* hook_name){
    int hook = get_hook();
    int num = get_hook_num(hook_name);
    if(hook>>num&1){
        hook ^= 1<<num;        
    }
    set_hook(hook);
}

void print_hook_res(char* hook_name, HookState hs){
    char msg[MAX_NAME + 20];
    if(hs == PASSED) sprintf(msg, "\033[1;32m%s----------------PASSED\033[0m\n", hook_name);
    if(hs == SKIPPED) sprintf(msg, "\033[1;33m%s----------------SKIPPED\033[0m\n", hook_name);
    if(hs == FAILED) sprintf(msg, "\033[1;31m%s----------------FAILED\033[0m\n", hook_name);
    printf(msg);
    return;
}

HookState check_hook(int hook_num, char* file_addres){
    HookState hs;
    if(hook_num == 0){
        hs = todo_check_hook(file_addres);
    }
    if(hook_num == 1){
        hs = eof_blank_space_hook(file_addres);
    }
    if(hook_num == 2){
        hs = format_check_hook(file_addres);
    }
    if(hook_num == 3){
        hs = balance_braces_hook(file_addres);
    }
    if(hook_num == 4){
        hs = indentation_check_hook(file_addres);
    }
    if(hook_num == 5){
        hs = static_error_check_hook(file_addres);
    }
    if(hook_num == 6){
        hs = file_size_check_hook(file_addres);
    }
    if(hook_num == 7){
        hs = character_limit_hook(file_addres);
    }
    if(hook_num == 8){
        hs = time_limit_hook(file_addres);
    }
    return hs;
}

int pre_commit_file(char* file_addres, int print_opt){
    if(!in_stage(file_addres) || find_in_file_list(get_file_list(get_current_stage_info_addres()), file_addres) == Delete){
        if(print_opt) print_fail("fail: file dosn't exist in stage area!");
        return 0;
    }
    int ok = 1;
    int hook = get_hook();
    if(print_opt) printf("\e[1m<<<<<<<<<<<<<<<<%s>>>>>>>>>>>>>>>>\e[m\n", get_rel_addres(file_addres));
    for(int hook_num = 0; hook_num < HOOK_NUMBER; hook_num++){
        if(hook>>hook_num&1){
            HookState hs = check_hook(hook_num, get_current_stage_changes_file_addres(get_name_file_in_stage_change(file_addres)));
            if(hs == FAILED) ok = 0;
            if(print_opt) print_hook_res(get_hook_name(hook_num), hs);
        }
    }
    return ok;
}

int pre_commit_hook(int print_opt){
    FileList* stage_file = get_file_list(get_current_stage_info_addres());
    int ok = 1;
    for(int i = 0; i < stage_file->cnt; i++){
        if(is_blanck(&stage_file->lst[i]) || stage_file->lst[i].state == Delete) continue;
        ok &= pre_commit_file(stage_file->lst[i].addres, print_opt);
    }
    return ok;
}

void pre_commit_resolve_file(char* file_addres){
    resolve_eof_blank_space(file_addres);
    resove_indentation_check(file_addres);
    resolve_balance_braces(file_addres);
}

int pre_commit_resolve(){
    FileList* stage_file = get_file_list(get_current_stage_info_addres());
    int ok = 1;
    for(int i = 0; i < stage_file->cnt; i++){
        if(is_blanck(&stage_file->lst[i]) || stage_file->lst[i].state == Delete) continue;
        pre_commit_resolve_file(get_current_stage_changes_file_addres(get_name_file_in_stage_change(stage_file->lst[i].addres)));
    }
    return ok;
}

int pre_commit_hook_file(int argc, char *argv[]){
    for(int i = 3; i < argc; i++){
        char *addres = cat_string(get_current_addres(), argv[i]);
        pre_commit_file(addres, 1);
    }
    return 1;
}

int pre_commit(int argc, char *argv[]){
    if(argc == 2){
        return pre_commit_hook(1);
    }
    if(!strcmp(argv[2], "hooks")){
        return show_hooks_list();
    }
    if(!strcmp(argv[2], "applied")){
        return show_applied_hooks();
    }
    if(!strcmp(argv[2], "add")){
        return add_hook(argv[4]);
    }
    if(!strcmp(argv[2], "remove")){
        return remove_hook(argv[4]);
    }
    if(!strcmp(argv[2], "-u")){
        return pre_commit_resolve();
    }
    if(!strcmp(argv[2], "-f")){
        return pre_commit_hook_file(argc, argv);
    }    
}