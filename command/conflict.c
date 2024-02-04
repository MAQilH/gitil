#include "../lib/lib.h"
#include "../model/file_list_model.h"
#include "diff.h"

int resolve_conflict(char* file_addres1, char* file_addres2, char* saved_addres, char* real_addres){
    FILE *saved_file = fopen(saved_addres, "w");
    print_warn(cat_string("<<<<<<<", cat_string(get_rel_addres(real_addres), ">>>>>>>")));
    int res = print_diff_file(file_addres1, 1, INF, file_addres2, 1, INF, saved_file);
    fclose(saved_file);
    return res;
}

void replace_conflict(FileList* added_files){
    for(int i = 0; i < added_files->cnt; i++){
        file_copy(
            get_saved_file_conflict_addres(added_files->lst[i].addres),
            added_files->lst[i].addres
        );
    }
}

int conflict(char* commit_id){
    char* current_commit = get_current_commit();

    FileList* current_files = get_commit_status_file(current_commit);
    FileList* conf_files = get_commit_status_file(commit_id);
    create_folder(get_conflict_temp_folder_addres());
    FileList* added_files = create_file_list(0);
    for(int i = 0; i < conf_files->cnt; i++){
        if(conf_files->lst[i].state == Delete) continue;
        if(find_index_in_file_list(current_files, conf_files->lst[i].addres) == -1 ||
            find_in_file_list(current_files, conf_files->lst[i].addres) == Delete) continue;
        if(!file_have_diff(
            get_commit_saved_file_addres(current_commit, current_files->lst[i].addres),
            get_commit_saved_file_addres(commit_id, current_files->lst[i].addres)
        )) continue;

        if(!resolve_conflict(
            get_commit_saved_file_addres(current_commit, conf_files->lst[i].addres),
            get_commit_saved_file_addres(commit_id, conf_files->lst[i].addres),
            get_saved_file_conflict_addres(conf_files->lst[i].addres),
            conf_files->lst[i].addres
        )){
            remove_directory(get_conflict_temp_folder_addres());
            return 0;
        }
        file_list_push_back(added_files, &conf_files->lst[i]);
    }
    replace_conflict(added_files);
    remove_directory(get_conflict_temp_folder_addres());
    return 1;
}