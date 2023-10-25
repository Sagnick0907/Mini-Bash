#include "headers.h"

void run_pwd() {
    char current_directory_path[1000];
    char *result=getcwd(current_directory_path, sizeof(current_directory_path));
    
    if (result != nullptr)  cout << current_directory_path << endl;
    else    perror("pwd: Cannot fetch path of working directory");
    
}
