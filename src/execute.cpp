/*
*
*   EXECUTOR: handles the call to other executables
*   
*/
#include "../include/execute.h"

void Executor::write_to_pipe(int file_descriptor, istream &input) {
    __gnu_cxx::stdio_filebuf<char> filebuf(file_descriptor, std::ios_base::out);
    ostream stream(&filebuf);
    string line;

    while(input.good()){
        getline(input, line);
        stream << line << '\n';
    }
}

void Executor::read_from_pipe (int file_descriptor, ostream &output) {
    __gnu_cxx::stdio_filebuf<char> filebuf(file_descriptor, std::ios_base::in);
    istream stream(&filebuf);
    string line;

    while(stream.good()) {
        getline(stream, line);
        output << line << '\n'; 
    }
}

void Executor::exec(char *cmd, istream &input, ostream &output) {

    // pipes for parent to write and read
    pipe(pipes[PARENT_READ_PIPE]);
    pipe(pipes[PARENT_WRITE_PIPE]);
     
    if(!fork()) {
        char *argv[]={ cmd };
 
        dup2(CHILD_READ_FD, STDIN_FILENO);  
        dup2(CHILD_WRITE_FD, STDOUT_FILENO);
 
        /// Close unused fds (child)
        close(CHILD_READ_FD);
        close(CHILD_WRITE_FD);
        close(PARENT_READ_FD);
        close(PARENT_WRITE_FD);
          
        execv(argv[0], argv);

    } else {

        // BUG: for some reason nothing works if this buffer isn't allocated with atleast
        //      2000 elements
        // WEIRD --> it is never used, so probably some memory bug
        char buffer[5000];
 
        // Close unused (parent)   
        close(CHILD_READ_FD);
        close(CHILD_WRITE_FD);

        // Write to child’s stdin
        write_to_pipe(PARENT_WRITE_FD, input);

        // Read from child’s stdout
        read_from_pipe(PARENT_READ_FD, output);
    }
}