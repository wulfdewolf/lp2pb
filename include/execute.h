#ifndef EXEC_H
#define EXEC_H

#include <unistd.h>
#include <ext/stdio_filebuf.h>
#include <iostream>

using namespace std;

#define NUM_PIPES          2
#define PARENT_WRITE_PIPE  0
#define PARENT_READ_PIPE   1

#define READ_FD  0
#define WRITE_FD 1
 
#define PARENT_READ_FD  ( pipes[PARENT_READ_PIPE][READ_FD]   )
#define PARENT_WRITE_FD ( pipes[PARENT_WRITE_PIPE][WRITE_FD] )
 
#define CHILD_READ_FD   ( pipes[PARENT_WRITE_PIPE][READ_FD]  )
#define CHILD_WRITE_FD  ( pipes[PARENT_READ_PIPE][WRITE_FD]  )

// EXECUTOR CLASS
class Executor {

    private:

    int outfd[2];
    int infd[2];
    int pipes[NUM_PIPES][2];

    void write_to_pipe(int file_descriptor, istream &input);
    void read_from_pipe (int file_descriptor, ostream &output);
    

    public:

    void exec(char *cmd, istream &input, ostream &output);

};

#endif