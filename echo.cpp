#include "headers.h"

void run_echo(vector<string> args)
{
    for (int i = 1; i<args.size(); i++){
        cout << args[i] << " ";
    }
    cout << endl;
}
