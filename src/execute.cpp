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

int Executor::exec(char *cmd, istream &input, ostream &output) {

    // pipes for parent to write and read
    pipe(pipes[PARENT_READ_PIPE]);
    pipe(pipes[PARENT_WRITE_PIPE]);

    pid_t c_pid;
    int status = 1;
    c_pid = fork();
     
    if(!c_pid) {
        char *argv[]={ cmd , NULL};
 
        dup2(CHILD_READ_FD, STDIN_FILENO);  
        dup2(CHILD_WRITE_FD, STDOUT_FILENO);
 
        /// Close unused fds (child)
        close(CHILD_READ_FD);
        close(CHILD_WRITE_FD);
        close(PARENT_READ_FD);
        close(PARENT_WRITE_FD);
          
        execvp(argv[0], argv);

    } else if(c_pid > 0){
 
        // Close unused (parent)   
        close(CHILD_READ_FD);
        close(CHILD_WRITE_FD);

        // Write to child’s stdin
        write_to_pipe(PARENT_WRITE_FD, input);

        // Wait for child
        wait(&status);

        // Read from child’s stdout
        read_from_pipe(PARENT_READ_FD, output);
    }
    if(WIFEXITED(status)) return WEXITSTATUS(status);
    else return 1;
}