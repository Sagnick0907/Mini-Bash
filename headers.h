// #ifndef headers
// #define headers
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <pwd.h>
#include <vector>
#include <sstream>
#include <dirent.h>
#include <csignal>
#include <iomanip>
#include <ctime>
#include <grp.h>
#include <signal.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <fstream>
#include <fcntl.h>
#include <cstring>

// #include "cd.cpp"
// #include "pwd.cpp"
// #include "echo.cpp"
// #include "search.cpp"
// #include "ls.cpp"
// #include "pinfo.cpp"
// #include "background.cpp"
// #include "redirection.cpp"

using namespace std;

void run_echo(vector<string> args);
void run_cd(vector<string> args, int arg_count);
void run_ls(vector<string> args, int arg_count);
void run_pwd();
void run_search(vector<string> args);
void run_with_redirection(vector<string> args, int argscount);
void run_pinfo(vector<string> args, int arg_count);

// #endif // headers