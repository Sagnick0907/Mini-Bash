#include "headers.h"


long calculate_total_blocks(string& directory) {
    long total_blocks = 0;
    // Open the directory
    DIR *directo;
    if(directory=="~"){
        struct passwd *pw = getpwuid(getuid());
        if (pw) {
            directo= opendir(pw->pw_dir);
        }
        else{
            perror("cd: Error reaching home directory!");
        }
    }
    else{
        directo= opendir(directory.c_str());
    }
    
    // Iterate through entries
    struct dirent *items;
    while ((items = readdir(directo)) != nullptr) {
        if (!items->d_name) {
            continue;
        }

        struct stat file_stat;
        if (stat((directory + "/" + items->d_name).c_str(), &file_stat) == 0) {
            total_blocks += file_stat.st_blocks;
        }
    }

    // Close the directory
    closedir(directo);
    return total_blocks;
}

void run_ls(vector<string> args, int arg_count){
    
    int flags = 0;
    string target_dir;
    vector<string> list_dir;

    // Parse flags and target directory/file
    for(int i = 1; i<arg_count; i++){
        if(args[i][0] == '-'){
            for (int j = 1; j <args[i].length(); j++){
                if (args[i][j] == 'a'){
                    flags |= 1;
                } 
                else if(args[i][j] == 'l') 
                {
                    flags |= 2;
                } 
                else{
                    cout << "ls: Unknown flag" << endl;
                    return;
                }
            }
        } 
        else{
            list_dir.push_back(args[i]);
        }
    }
    
    if (list_dir.size() == 0) {
        // Open the directory
            DIR *directo=opendir(".");
            
            if (directo == nullptr) {
                perror("ls: Directory cannot be opened!");
                return;
            }
            char current_directory[1000];
            string cwd;

            if (getcwd(current_directory, sizeof(current_directory)) != nullptr) {
                cwd=current_directory ;
            }
            else{
               perror("Error getting current directory!");
            }
            
            if(flags ==2 || flags==3){
                long total_blocks = calculate_total_blocks(cwd);
                cout<< "total " << total_blocks/2 << endl; // Blocks are in 512-byte units 
            }
            // Iterate through the list of entries
            struct dirent *items;
            while ((items = readdir(directo)) != nullptr) {
                if (items->d_name[0] == '.' && !(flags & 1)) {
                    continue;
                }

                if (flags & 2) {
                    struct stat stat_mode;
                    if (stat(items->d_name, &stat_mode) == 0) {
                        struct tm *timestatus;
                        char localbuff[80];
                        timestatus = localtime(&stat_mode.st_mtime);
                        strftime(localbuff, sizeof(localbuff), "%b %d %H:%M", timestatus);
                        struct passwd *user_info=getpwuid(stat_mode.st_uid);
                        struct group *group_info=getgrgid(stat_mode.st_gid);



                        cout<< (S_ISDIR(stat_mode.st_mode) ? "d" : "-")
                            << (stat_mode.st_mode & S_IRUSR ? "r" : "-")
                            << (stat_mode.st_mode & S_IWUSR ? "w" : "-")
                            << (stat_mode.st_mode & S_IXUSR ? "x" : "-")
                            << (stat_mode.st_mode & S_IRGRP ? "r" : "-")
                            << (stat_mode.st_mode & S_IWGRP ? "w" : "-")
                            << (stat_mode.st_mode & S_IXGRP ? "x" : "-")
                            << (stat_mode.st_mode & S_IROTH ? "r" : "-")
                            << (stat_mode.st_mode & S_IWOTH ? "w" : "-")
                            << (stat_mode.st_mode & S_IXOTH ? "x" : "-")
                            << " "
                            << setw(2) << stat_mode.st_nlink
                            << " "
                            << user_info->pw_name  // User name
                            << " "
                            << group_info->gr_name  // Group name
                            << " "
                            << setw(8) << stat_mode.st_size
                            << " "
                            << localbuff
                            << " "
                            << items->d_name
                            << endl;
                    }
                }
                else{
                    cout << items->d_name << " ";
                }
            }
            cout << endl;
            closedir(directo);
    }
    else{  
        for(int i=0;i<list_dir.size();i++){
            cout<< list_dir[i]<<endl;
            // Open the directory
            if(flags ==2 || flags==3)
            {
                long total_blocks = calculate_total_blocks(list_dir[i]);
                cout<< "total " << total_blocks / 2 << endl; 
            }

            DIR *directo;
            if(list_dir[i]=="~"){
                struct passwd *pw = getpwuid(getuid());
                if (pw) {
                    directo= opendir(pw->pw_dir);
                }
                else{
                    perror("cd: Error reaching home directory!");
                }
            }
            else{
                directo= opendir(list_dir[i].c_str());
            }

            
            if(directo == nullptr){
                perror("ls: Directory cannot be opened!");
                return;
            }

            // Iterate through entries
            struct dirent *items;
            while ((items = readdir(directo)) != nullptr) {
                if (!items->d_name || items->d_name[0] == '.' && !(flags & 1)) {
                    continue;
                }

                if (flags == 2 || flags==3) 
                {
                    struct stat stat_mode;
                    string full_path = string(list_dir[i]) + "/" + items->d_name;
                    if (stat(full_path.c_str(), &stat_mode) == 0)
                    {
                        
                        struct tm *timestatus;
                        char localbuff[80];
                        timestatus = localtime(&stat_mode.st_mtime);
                        strftime(localbuff, sizeof(localbuff), "%b %d %H:%M", timestatus);
                        
                        struct passwd *user_info = getpwuid(stat_mode.st_uid);
                        struct group *group_info = getgrgid(stat_mode.st_gid);
                        


                        cout<< (S_ISDIR(stat_mode.st_mode) ? "d" : "-")
                            << (stat_mode.st_mode & S_IRUSR ? "r" : "-")
                            << (stat_mode.st_mode & S_IWUSR ? "w" : "-")
                            << (stat_mode.st_mode & S_IXUSR ? "x" : "-")
                            << (stat_mode.st_mode & S_IRGRP ? "r" : "-")
                            << (stat_mode.st_mode & S_IWGRP ? "w" : "-")
                            << (stat_mode.st_mode & S_IXGRP ? "x" : "-")
                            << (stat_mode.st_mode & S_IROTH ? "r" : "-")
                            << (stat_mode.st_mode & S_IWOTH ? "w" : "-")
                            << (stat_mode.st_mode & S_IXOTH ? "x" : "-")
                            << " "
                            << setw(2) << stat_mode.st_nlink
                            << " "
                            << user_info->pw_name  // User name
                            << " "
                            << group_info->gr_name  // Group name
                            << " "
                            << setw(8) << stat_mode.st_size
                            << " "
                            << localbuff
                            << " "
                            << items->d_name
                            << endl;
                    }
                }
                else{
                    cout << items->d_name << " ";
                }
            }
            cout << endl;

            closedir(directo);
        }
        
    }

}