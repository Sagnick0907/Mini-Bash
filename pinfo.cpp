#include "headers.h"

// Function to print process information based on the provided PID
void print_currprocess_info(pid_t pid){
    char proc_path[1000];
    string proc="/proc/"+to_string(pid)+"/exe";
    snprintf(proc_path, sizeof(proc_path), proc.c_str());

    ifstream status_file(proc_path);
    if (!status_file) {
        cerr << "pinfo: Failed to open process status file." << endl;
        return;
    }

    string line;
    string process_status, memory, executable_path;
    bool found_memory = false;

    while (getline(status_file, line)){
        if (line.compare(0, 3, "State:") == 0){
            process_status = line.substr(7);
        }
        else if(line.compare(0, 8, "VmSize:") == 0)
        {
            memory = line.substr(7);
            found_memory = true;
            break; // We found the memory information, no need to continue searching
        }
    }

    // Determine if the process is in the foreground or background
    string PSC = "{";
    char stat_char = process_status[0]; // First character of process status
    if (stat_char == 'R' || stat_char == 'S')
    {
        PSC += stat_char;
        if (pid == getpid())
        {
            PSC += "+"; // Running in the foreground
        }
    } 
    else if(stat_char == 'T'){
        PSC += "T"; // Stopped (on a signal)
    } 
    else if(stat_char == 'Z'){
        PSC += "Z"; // Zombie
    } 
    else{
        PSC += "?"; // Unknown status
    }
    PSC += "}";

    // Get the executable path from /proc/<pid>/exe
    char exe_path[1000];
    string exe="/proc/" +to_string(pid)+"/exe";
    snprintf(exe_path, sizeof(exe_path), exe.c_str());
    char exec_path[1000];
    ssize_t len = readlink(exe_path, exec_path, sizeof(exec_path) - 1);

    if (len != -1) {
        exec_path[len] = '\0';
        executable_path = exec_path;
    }
    else{
        cerr << "pinfo: Failed to read executable path." << endl;
    }

    cout << "pid -- " << pid << endl;
    cout << "Process Status -- " << PSC << endl;
    if (!found_memory){
        cerr << "memory -- " << "Failed to find memory information." << " {Virtual Memory}" << endl;
    }
    else{
        cout << "memory -- " << memory << " {Virtual Memory}" << endl;
    }
    cout << "Executable Path -- " << executable_path << endl;
}

void run_pinfo(vector<string> args, int arg_count) {
    if (arg_count==1) {
        // If no PID is provided, print process info for the shell
        print_currprocess_info(getpid());
    }
    else if(arg_count==2) {
        // Parse PID and print process info
        
        //int pid = stoi(args[1]);
        int pid;
        try {
            pid = stoi(args[1]);
        } 
        catch (const invalid_argument& e){
            cerr << "Invalid PID: " << args[1] << endl;
            return;
        }
        catch (const out_of_range& e){
            cerr << "PID out of range: " << args[1] << endl;
            return;
        }
        // Check if the PID exists
        if (kill(pid, 0) == 0){
            print_currprocess_info(pid);
        }
        else {
            cerr << "Process with PID " << pid << " does not exist." << endl;
        }
    }
    else{
        cerr<<"Too many arguments!"<<endl;
        return;
    }

}