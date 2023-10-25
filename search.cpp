#include "headers.h"

bool search_recursive(const char *target_name, const char *current_dir_path) {
    struct dirent *element;
    DIR *current_directory = opendir(current_dir_path);

    if (current_directory==nullptr){
        perror("opendir: Current Directory cannot be opened");
        return false;
    }

    while ((element = readdir(current_directory)) != nullptr){
        if (strcmp(element->d_name, target_name)==0){
            closedir(current_directory);
            return true;
        }

        if (element->d_type == DT_DIR && strcmp(element->d_name, ".") != 0 && strcmp(element->d_name, "..")!=0)
        {
            char inner_directory[1000];
            snprintf(inner_directory, sizeof(inner_directory), "%s/%s", current_dir_path, element->d_name);
            if (search_recursive(target_name, inner_directory)){
                closedir(current_directory);
                return true;
            }
        }
    }

    if (closedir(current_directory) == -1) {
        perror("closedir Current Directory cannot be closed");
    }
    return false;
}

void run_search(vector<string> args) {
    if (args.size()<=1) {
        cerr << "search: Missing argument" << endl;
        return;
    }

    if (search_recursive(args[1].c_str(), ".")) cout << "True" << endl;
    else    cout << "False" << endl;
    
}
