void checkout_branch(char* branch_name, int force);
int checkout_commit(char* commit_id, int force);
int checkout(int argc, char* argv[]);
char* get_HEAD_x_commit_id(int n);