void create_folder(char* addres);
void create_hidden_folder(char* addres);
int check_in_root();
int exist_folder(char* addres);
int exist_file(char* addres);
void create_file(char* name, char* addres);
int file_cmp(char* file_addres1, char* file_addres2);
int is_directory(char *path);
void upate_middle_file(void* data, int size, int index, char* file_addres);
int remove_directory(char *path);
int file_copy(char FileSource[], char FileDestination[]);
void pop_from_file(int size, char* file_addres);
int check_exist_in_folder(char *addres, char *folder_addres);