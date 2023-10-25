#include "headers.h"

// Function to run a command with input and output redirection
void run_with_redirection(vector<string> args, int argscount) {
    const char *input_file = nullptr;
    const char *output_file = nullptr;
    bool append = false;
    // int twooperators=0;

    // Check for input and output redirection
    for (int i = 1; i <args.size(); i++){
        if (args[i]=="<") {
            // Input redirection
            if (i + 1 < args.size()){
                input_file = args[i + 1].c_str();
                args.erase(args.begin() + i, args.begin() + i + 2);     // remove last 2 strings
                if(i<args.size()){
                    output_file=args[i].c_str();
                    args.pop_back();
                }
            } 
        }
            
        else if (args[i]==">"){
            // Output redirection (overwrite)
            if (i + 1 < args.size()) {
                output_file = args[i + 1].c_str();
                args.erase(args.begin() + i, args.begin() + i + 2); // Remove ">" and the output filename
            }
            
        } 
        else if (args[i]== ">>"){
            // Output redirection (append)
            if (i + 1 < args.size()) {
            output_file = args[i + 1].c_str();
                args.erase(args.begin() + i, args.begin() + i + 2); // Remove ">>" and the output filename
                append = true;
            }
        }

    }

    pid_t pid;
    int status;

    pid = fork();

    if (pid == 0) {
        // Child process
        if(input_file != nullptr){
            // Redirect stdin to input file
            int fd = open(input_file, O_RDONLY);
            if (fd == -1) {
                perror("shell");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        if(output_file != nullptr){
            // Redirect stdout to output file
            int fd;
            if (append)
                fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            
            else
                fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            
            if (fd == -1) {
                perror("shell");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        char** chararg = new char*[args.size()+1];
        // Populate the charArray with C-string representations
        for (size_t i = 0; i < args.size(); ++i) {
            chararg[i] = const_cast<char*>(args[i].c_str());
        }
        chararg[args.size()] = nullptr;

        // Execute the command using execvp
        if (execvp(args[0].c_str(), chararg) == -1) {
            perror("shell");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0){
        perror("shell");
    } else {
        // Parent process
        // Wait for the child process to finish in the foreground
        do{
            waitpid(pid, &status, WUNTRACED);
        }
        while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    cout<<endl;
}