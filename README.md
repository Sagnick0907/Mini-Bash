# Advanced Operating Systems Assignment-2
### | Author : Sagnick Bhar | Roll No. : 2023201008 |  

## TASK :  
Implement a shell that supports a semi-colon separated list of commands. Use 'strtok' to
tokenize the command. Also, support '&' operator which lets a program run in the background
after printing the process id of the newly created process. Write this code in a modular fashion.
The goal of the assignment is to create a user-defined interactive shell program using
c/cpp that can create and manage new processes. The shell should be able to create a
process out of a system program like emacs, vi, or any user-defined executable. Your
shell can handle background and foreground processes and also input/output
redirections and pipes.
The following are the specifications for the
### Terminal Input:  
`
$  make
`  
` 
$ ./main  
`

## Task 1:
Display requirement:  
 When you execute your code, a shell prompt of the following form
must appear along with it. Do NOT hard-code the user name and system name here.

### Solution:
1. void display_usersystem_prompt() function is used to creat the shell prompt appearing on the LHS in the terminal. 
2. Used getlogin_r() to obtain username and gethostname() to obtain system name and displayed it as per the requirement.


## Task 2:
cd, echo, pwd:  
 If the user executes "cd" i.e changes the directory, then the corresponding change must be reflected in the shell as well. If your current working directory is the directory from which your shell is invoked, then on executing command "cd .." your shell should display the absolute path of the current directory from the root.

### Solution:
1. void display_usersystem_prompt() function is used to creat the shell prompt appearing on the LHS in the terminal. 
2. Used getlogin_r() to obtain username and gethostname() to obtain system name and displayed it as per the requirement.
### Terminal Input: 
`
$  cd
`  
` 
$  echo "I am Sagnick Bhar"
`   
` 
$  pwd
` 
### Solution:  
1. For cd handled all formats of input (used chdir function):
    - Handled error related to incorrect number of arguments.  
    - cd   : enter home directory.
    - cd - : enter previously entered directory if it exists.(Maintained a previous_directory named variable and updated it everytime any cd commands is executed.)
    - cd ~ : enter home directory.
    - cd .. : Enter parent directory.
    - cd . : Enter current directory.
    - cd target_dir : Enter user input given target directory. 
2. echo:  
    - Simply print out the command except the "echo" string. 
3. pwd: 
    - Used getcwd to obtain the current directory and handled any error.


## Task 3:
ls:  
Implement the ls command with its two flags “-a” and “-l”. You should be able to
handle all the following cases also:
● ls  
● ls -a  
● ls -l  
● ls .  
● ls ..  
● ls ~  
● ls -a -l  
● ls -la 
● ls -al  
● ls <Directory/File_name>  
● ls -<flags> <Directory/File_name>  
● ls -<flags> <Directory1/File_name1> <Directory2/File_name2> ...

### Terminal Input: 
`
$  ls 
` 
### Solution:  
1. For ls handled the following formats of input (created flags for handling all options: a,l and dirent.h to read all the entries of the given user input directory. ):
    - Used a for loop to handle error like unknown flags, update flags(for a,l) and create a list of directories given as input from user.
    - First check if no directory input is given then select current directory, else create a for loop for iterating through all the list of directories given as an input from the user.  
    - Now, for each directory, 
        - Open the directory.
        - Print the total number of blocks if flag=2 or 3.
        - Used to iterate through the list of entries using dirent.h and calculated all the file/directories permissions, username, group name, date and time when file was created and modified, and the entry name.
        - Close the directory.

## Task 4:
System commands (background/fg), with and without arguments:  
Foreground processes: For example, executing a "vi" command in the foreground implies that your shell will wait for this process to complete and regain control when this process exits.
Background processes: Any command invoked with "&" is treated as a background command. This implies that your shell will spawn that process and doesn't wait for the process to exit. It will keep taking other user commands. Whenever a new background process is started, print the PID of the newly created background process on your shell also.

### Terminal Input: 
`
$  <Name@UBUNTU:~> gedit &
` 

### Solution:  
1. Firstly created a vector where all the process ids of running background_processes are stored.
2. If a command ends with '&', then call the function run_background_command() nad pass the command after removing '&'.
3. Call fork to create a new child process where the command given as input is called using execvp() function, its, pid is stored in the vector and it keeps on going in the background (special cases for gedit is handled where if more than 1 text files get opened then separate tezxt files will get opened).
4. In the main function, at the end of infinite while loop and while exiting check if the background process is still running. If it has ended remove its pID from the vector.

## Task 5:
pinfo:  
This prints the process-related info of your shell program. Use of “popen()” for implementing pinfo is NOT allowed.

### Terminal Input: 
`
$  pinfo
`   
`
$  pinfo 7777
` 

### Solution:  
1. Firstly we check if the argument format is correct or not.
2. In Ubuntu for every process a file is created in a path like this ("/proc/process_id/exe").
3. So all the process related information like pid, process status code, virtual memory and exevutable path are present there, which we extract it and then print it. 

## Task 6:
search:  
Search for a given file or folder under the current directory recursively. Output should be True or False depending on whether the file or folder exists

### Terminal Input: 
`
$  search xyz.txt
` 

### Solution:  
1. Used dirent.h library to access all the contents of the current directory.
2. We run a while loop to compare all the entries in the directory with the target file. If there is a directory inside the CWD then we recursively call the search_recursive() function on that directory to find  the target file and return false each time if not found.

## Task 7:
I/O redirection:  
Using the symbols <, > and >>, the output of commands, usually written to stdout, can be redirected to another file, or the input taken from a file other than stdin. Both input and output redirection can be used simultaneously. Your shell should support this functionality. Your shell should handle these cases appropriately: An error message
should be displayed if the input file does not exist. The output file should be created (with permissions 0644) if it does not already exist. In case the output file already exists, it should be overwritten in case of > and appended to in case of >> .

### Terminal Input: 
`
$  echo "hello" > output.txt
`   
`
$  cat < example.txt
` 
`
$  sort < file1.txt > lines_sorted.txt
` 

### Solution:  
1. Created run_with_redirection() function to execute commands involving redirection.
2. Keep ready character arrays for storing input file and output file.
3. Run a for loop to parse and check what operators are there in the command (<, >, >> ) and accordingly store the input file name and output file name in the respective variables.
4. Create files according to the required name, permissions and modes  :
    - for '<' take input from the given file and feed it to the command before it and execute it with execvp()
    - for '>' take the output from the command executed using execvp() and store it in a newly created/replaced output file.
    - for '>' take the output from the command executed using execvp() and append it to the output file.
5. Close the file pointers.

### Status:
Command with more than 1 operators such as "sort < file1.txt > lines_sorted.txt" is not implemented yet.

## Task 8:
pipeline:  
A pipe, identified by |, redirects the output of the command on the left as input to the command on the right. One or more commands can be piped as the following examples show. Your program must be able to support any number of pipes.  

### Terminal Input: 
`
$  cat file.txt | wc
` 
`
$  cat sample2.txt | head -7 | tail -5
` 
`
$  sort < file1.txt > lines_sorted.txt
` 

### Solution:  

### Status:
Not implemented yet.

## Task 9:
Redirection with pipeline:   
Input/output redirection can occur within command pipelines, as the examples below show. Your shell should be able to handle this.

### Terminal Input: 
`
$  ls | grep *.txt > out.txt cat < in.txt | wc -l > lines.txt
` 

### Solution:  

### Status:
Not implemented yet.

## Task 10:
1. CTRL-Z: It should push any currently running foreground job into the background,
and change its state from running to stopped. This should have no effect on the
shell if there is no foreground process running.  
2. CTRL-C It should interrupt any currently running foreground job, by sending it the
SIGINT signal: This should have no effect on the shell if there is no foreground
process running.  
3. CTRL-D: It should log you out of your shell, without having any effect on the
actual terminal.

### Terminal Input: 
Press Ctrl+Key

### Solution:  
1.  The cin.eof() is used to check if an end-of-file condition (Ctrl+D) was encountered while reading input.  Then save the command history and exit.

### Status:
1. CTRL-D is implemented.  

## Task 11:
Autocomplete for all the files/directories under the current directory.:  
Pressing the TAB key should either complete the command or output a list of matching files/dirs
(space separated) if there are more than one.
Example: # assume alpha.txt, alnum.txt are 2 files in your current path:  

### Terminal Input: 
`
$  cat alp<TAB>: completes ‘ha.txt’
` 
`
$  cat al<TAB>
` 


### Solution:  

### Status:
Not implemented yet.

## Task 12:
history:  
Implement a ‘history’ command which is similar to the actual history command. The maximum number of commands it should output is 10. The maximum number ofcommands your shell should store is 20. You must overwrite the oldest commands if more than 20 commands are entered. You should track the commands across all
sessions and not just one.

### Terminal Input: 
`
$  history
`   
`
$  history 4
` 

### Solution:  
1. Created a history_file to store all the commands used recently in a circular array fashion.
2. Everytime the terminal opens the list gets loaded using load_history_from_file() and for every command entered the list gets updates using add_to_history() function.
3. Whenever history command is given as input display_history() function displays the top recently used commands.
4. Before the program ends, all the changes are saved to history_file using save_history_to_file() function.


### Status:
Can display top 10 by default or 'num' number of  commands used recently if so many exists out of the 20 commands throughout all sessions.
Part (b) Up Arrow Key: is not yet implemented.



