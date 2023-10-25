#include "headers.h"

string previous_directory="";

void run_cd(vector<string> args, int arg_count){
    if (arg_count==1) {
        struct passwd *pw = getpwuid(getuid());
        if(pw){
            char current_directory[1000];
            if (getcwd(current_directory, sizeof(current_directory)) != nullptr){
                previous_directory=current_directory;
            }
            else{
                perror("pwd");
            }
            chdir(pw->pw_dir);
        }
        else{
            perror("Error reaching Home Directory!");
        }
    }
    else{
        // More than 1 argument
        if (arg_count>2) {
            printf("Invalid number of arguments! Please retype again.\n");
            return;
        }
        string target_dir = args[1];
        
        if(target_dir=="-"){
            if(previous_directory.length() > 0){
                char current_directory[1000];
                string temp;
                if (getcwd(current_directory, sizeof(current_directory)) != nullptr) {
                    temp=current_directory;
                } 
                else{
                    perror("Error reaching current directory!");
                }
                //cout<<previous_directory;
                chdir(previous_directory.c_str());
                previous_directory=temp;
                //cout << previous_directory << endl;
            }
            else{
                cout << "cd: Previous directory not available" << endl;
            }
        } 
        else if(target_dir=="~"){
            struct passwd *pw = getpwuid(getuid());
            if(pw){
                char current_directory[1000];
                if (getcwd(current_directory, sizeof(current_directory)) != nullptr) {
                    previous_directory=current_directory;
                } 
                else{
                    perror("Error updating previous directory!");
                }
                if (chdir(pw->pw_dir) != 0) {
                    perror("cd");
                }
            }
            else{
                perror("cd: Error reaching home directory!");
            }
        } 
        else if(target_dir==".."){
            // Move up to the parent directory
            char current_directory[1000];
            if (getcwd(current_directory, sizeof(current_directory)) != nullptr) {
                previous_directory=current_directory;
            } 
            else{
                perror("Error updating previous directory!");
            }
            if(chdir("..") != 0){
                perror("cd: Error reaching parent directory!");
            }
        } 
        else if(target_dir==".")
        {
            char current_directory[1000];
            if (getcwd(current_directory, sizeof(current_directory)) != nullptr) {
                previous_directory=current_directory;
            }
            else {
                perror("Error updating previous directory!");
            }

            if (chdir(target_dir.c_str()) != 0) {
                perror("cd: Error reaching current directory!");
            }
        }
        else{
            char current_directory[1000];
            if (getcwd(current_directory, sizeof(current_directory)) != nullptr) {
                previous_directory=current_directory;
            }
            else{
                perror("Error updating previous directory!");
            }
            if (chdir(target_dir.c_str()) != 0) {
                perror("cd: Error reaching target directory!");
            }
        }
    }
}