#include "headers.h"

// Function to obtain the terminal dimensions for formatting output
pair<int, int> retrieve_terminal_size() {
    struct winsize terminal_info;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal_info);
    return {terminal_info.ws_row, terminal_info.ws_col};
}


pid_t foreground_process_pidlist = -1;     // Store PID of foreground process
vector<pid_t> background_processes_pidlist; // Store PIDs of background processes

//------------- Signal Handling --------------------------------------------------

// Signal handler to reap terminated background processes
void handle_sigchld(int signum) {
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        // Process with PID 'pid' has terminated
        auto it = background_processes_pidlist.begin();
        while (it != background_processes_pidlist.end()) {
            if (*it == pid) {
                it = background_processes_pidlist.erase(it);
            } else {
                ++it;
            }
        }
    }
}

void handle_ctrl_d(int signum) {
    // Handle Ctrl-D (EOF) signal
    cout << endl;
    exit(0);      // Exit the program
}

//--------------------------------------------------------------------------------

//------------- Background Process Handling --------------------------------------
const char *output_directory = "/tmp/gedit_output_AOSAssignment2/"; // Directory for output files
int background = 0;

// Function to create the output directory if it doesn't exist
void create_output_file(){
    struct stat st;
    if(stat(output_directory, &st) == -1){
        if(mkdir(output_directory, 0700)!=0){
            cerr << "Failed to create the output directory." << endl;
            return;
        }
    }

}

void run_background_command(vector<string> args, int background)
{
    pid_t pid, wpid;
    int status;

    //foreground_process_pidlist = getpid(); // Set the foreground process PID

    pid = fork();

    if(pid == 0){
        // Child process
        // Create the output directory if it doesn't exist
        create_output_file();

        // Generate a unique filename based on timestamp
        time_t current_time=time(nullptr);
        stringstream ss;
        ss << "gedit_output_" << current_time << ".txt";
        string output_filename=ss.str();

        // Execute the command with output redirection to the unique filename
        args.push_back(output_filename);

        char** chararg = new char*[args.size()+1];
        // Populate the charArray with C-string representations
        for (size_t i = 0; i < args.size(); ++i)
        {
            chararg[i] = const_cast<char*>(args[i].c_str());
        }
        chararg[args.size()]=nullptr;

        // Check if the command contains 'gedit'
        bool contains_gedit = false;
        for (int i = 0; i < args.size(); i++){
            if(args[i]=="gedit"){
                contains_gedit = true;
                break;
            }
        }

        // If 'gedit' is not in the command, add a dummy filename
        if(!contains_gedit){
            // Execute the command using execvp
            if(execvp(args[0].c_str(), chararg) == -1){
                perror("shell");
                exit(EXIT_FAILURE);
            }
        }
        else{
            string cwd;
            char current_directory[1024];
            if (getcwd(current_directory, sizeof(current_directory)) != nullptr) {
                cwd=current_directory;
            } else {
                perror("cwd");
            }

            // Change the working directory to the output directory
            chdir(output_directory);
            // Execute the command using execvp
            if (execvp(args[0].c_str(), chararg) == -1) {
                perror("shell");
                exit(EXIT_FAILURE);
            }

            chdir(cwd.c_str());
        }

    }
    else if (pid < 0) {
        perror("shell");
    }
    else {
        // Print the PID of the background process and add it to the list
        cout << "["<< background<<"] "<< pid << endl;
        background_processes_pidlist.push_back(pid);
    }
}

void kill_background_processes(int background){
    int status;
    int size=background_processes_pidlist.size();
    for (size_t i = 0; i < size; i++){
        pid_t pid = waitpid(background_processes_pidlist[i], &status, WNOHANG);
        if (pid > 0){
            // Process with PID 'pid' has terminated
            background_processes_pidlist.erase(background_processes_pidlist.begin() + i);
            size--;
            background--;
        }
    }
}
//--------------------------------------------------------------------------------

//------------------ Implementing History command --------------------------------
// Circular buffer for command history
vector<string> command_list(20);
int historylist_size = 0; // Number of commands in history
int hindex = 0; // Index for adding new commands to history

// File to store the command history
const char *history_file ="command_listfile.txt";

// Function to load command history from a file
void load_historylist_from_file(){
    ifstream file(history_file);
    if (!file.is_open()){
        cerr << "shell: Could not open command history file." << endl;
        return;
    }

    string command;
    while (getline(file, command)){
        if (historylist_size < 20){
            // History is not full, simply add the command
            command_list[hindex] = command;
            historylist_size++;
            hindex = (hindex + 1) % 20;
        }
        else{
            // History is full, overwrite the oldest command
            command_list[hindex]=command;
            hindex=(hindex + 1)%20;
        }
    }

    file.close();
}

// Function to save command history to a file
void save_historylist_to_file() {
    ofstream file(history_file);
    if (!file.is_open()) {
        cerr << "shell: Could not open command history file for writing." << endl;
        return;
    }

    int start_index = (hindex - historylist_size + 20) % 20;
    int end_index = (hindex + 20) % 20;

    int save_count = min(historylist_size, 20);
    for (int i = 0; i < save_count; i++) {
        file << command_list[(start_index + i) % 20] << endl;
    }

    file.close();
}

// Function to add a command to the command history
void modify_history_list(const string& command) {
    if (historylist_size < 20) {
        // History is not full, simply add the command
        command_list[hindex] = command;
        historylist_size++;
        hindex = (hindex + 1) % 20;
    } else {
        // History is full, overwrite the oldest command
        command_list[hindex] = command;
        hindex = (hindex + 1) % 20;
    }

    // Save the updated history to the file
    save_historylist_to_file();
}

// Function to display the command history
void display_history(vector<string> args){
    int start_index = (hindex - historylist_size + 20) % 20;
    int end_index = (hindex + 20) % 20;

    if(args.size()==1){
        int display_count = min(historylist_size, 10);
        for(int i=end_index-display_count; i<end_index; i++){
            cout << command_list[(start_index + i) % 20] << endl;
        }
    }
    else{
        int display_count = min(historylist_size, stoi(args[1]));
        for(int i=end_index-display_count; i<end_index; i++){
            cout << command_list[(start_index + i) % 20] << endl;
        }
    }
    
}

//--------------------------------------------------------------------------------

void display_usersystem_prompt(){
    char username[1000];
    char system_name[1000];
    char current_dir[1000];

    getlogin_r(username, sizeof(username));
    gethostname(system_name, sizeof(system_name));
    getcwd(current_dir, sizeof(current_dir));
    string cwd=current_dir;
    size_t found = cwd.find("/home/");
    if (found != string::npos){
        cwd.replace(found, 5, "~");
    }
    strcpy(current_dir, cwd.c_str());

    //printf("%s@%s:%s> ", username, system_name);
    printf("%s@%s:%s> ", username, system_name, current_dir);
}

int search_in_args(vector<string> args, string target){

    for(int i=0;i<args.size();i++){
        if(args[i]==target) return i;
    }

    return -1;
}

vector<string> tokenize(string str, char c){
    vector<string> tok;
    int i=0;
    for(;i<str.length();i++){
        if(str[i]!=' '){
           break;
        }
    }
    str=str.substr(i);
    stringstream ss(str);
    string temp_str;

    while(getline(ss, temp_str, c)){
        tok.push_back(temp_str);
    }

    return tok;
}

int main() {
    // Load command history from the file
    load_historylist_from_file();

    // Set up signal handler for SIGCHLD to reap terminated background processes
    struct sigaction sa;
    sa.sa_handler = handle_sigchld;
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa, nullptr);

    // Set up signal handler for CTRL-D (EOF)
    struct sigaction ctrl_d_sa;
    ctrl_d_sa.sa_handler = handle_ctrl_d;
    ctrl_d_sa.sa_flags = 0;
    sigaction(SIGINT, &ctrl_d_sa, nullptr);


    char input[1000];
    const char delimt = ';';
    const char arg_delimt = ' ';

    while(1) {
        display_usersystem_prompt();

        cin.getline(input, sizeof(input));

        // Detect if user has pressed Ctrl+D
        if (cin.eof()) {
            save_historylist_to_file();
            cout << endl;
            exit(EXIT_SUCCESS);       // Exit the shell
        }

        input[strcspn(input, "\n")] = '\0';

        vector<string> cmds=tokenize(input, delimt);
        int noOfCmds = cmds.size();
        int t=0;


        while (t != noOfCmds) {

            vector<string> args=tokenize(cmds[t], arg_delimt);

            int arg_count = args.size();
            int cnt=0;

            if (arg_count > 0) {
                if((search_in_args(args,"<")!=-1 || search_in_args(args,">")!=-1  || search_in_args(args,">>")!=-1) && args[arg_count - 1]!="&"){
                    // Execute the command with input and output redirection
                    modify_history_list(cmds[t]);
                    run_with_redirection(args, arg_count);
                }
                else if (args[0]=="pwd" && args[arg_count - 1]!="&") {
                    modify_history_list(cmds[t]);
                    run_pwd();
                }
                else if (args[0]=="echo" && args[arg_count - 1]!="&") {
                    modify_history_list(cmds[t]);
                    run_echo(args);
                }
                else if(args[0]=="search" && args[arg_count - 1]!="&"){
                    modify_history_list(cmds[t]);
                    run_search(args);
                }
                else if(args[0]=="ls" && args[arg_count - 1]!="&"){
                    modify_history_list(cmds[t]);
                    run_ls(args, arg_count);
                }
                else if(args[0]=="cd" && args[arg_count - 1]!="&"){
                    modify_history_list(cmds[t]);
                    run_cd(args, arg_count);
                }
                else if (args[0]=="pinfo" && args[arg_count - 1]!="&") {
                    modify_history_list(cmds[t]);
                    run_pinfo(args, arg_count);
                }
                else if (search_in_args(args,"|")!=-1 && args[arg_count - 1]!="&") {
                    modify_history_list(cmds[t]);
                    cout<<"Not implemented yet!"<<endl;
                    // run_with_redirection(args);
                }
                else if (args[0]=="history" && args[arg_count - 1]!="&") {
                    modify_history_list(cmds[t]);
                    // Display command history
                    display_history(args);
                }
                else if (args[0]=="exit" && args[arg_count - 1]!="&") {
                    modify_history_list(cmds[t]);
                    // Check for background processes that have terminated
                    kill_background_processes(background);
                    save_historylist_to_file();
                    // exit
                    exit(EXIT_SUCCESS);
                }
                else if(arg_count > 1 && args[arg_count - 1]=="&"){
                    modify_history_list(cmds[t]);
                    // If the last argument is "&", it's a background command
                    args[arg_count - 1] = "\0"; // Remove the "&" argument
                    background += 1;

                    run_background_command(args, background);
                }

                else{
                    cout<<"Invalid Command!"<<endl;
                }
            }

            t++;
        }
        // Check for background processes that have terminated and remove them
        kill_background_processes(background);

    }
    save_historylist_to_file();

    return 0;
}