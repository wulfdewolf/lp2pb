/*
*
*   EXECUTION: handles the call to other executables
*
*   
*/
#include "../include/exec.h"

void Executor::exec(char *cmd, string input) {

    // pipes for parent to write and read
    pipe(pipes[PARENT_READ_PIPE]);
    pipe(pipes[PARENT_WRITE_PIPE]);
     
    if(!fork()) {
        char *argv[]={ cmd };
 
        dup2(CHILD_READ_FD, STDIN_FILENO);  
        dup2(CHILD_WRITE_FD, STDOUT_FILENO);
 
        /* Close fds not required by child. Also, we don't
           want the exec'ed program to know these existed */
        close(CHILD_READ_FD);
        close(CHILD_WRITE_FD);
        close(PARENT_READ_FD);
        close(PARENT_WRITE_FD);
          
        execv(argv[0], argv);

    } else {
        char buffer[100];
        int count;
 
        /* close fds not required by parent */       
        close(CHILD_READ_FD);
        close(CHILD_WRITE_FD);

        // Write to child’s stdin
        write(PARENT_WRITE_FD, input.c_str(), 5);

        // Read from child’s stdout
        count = read(PARENT_READ_FD, buffer, sizeof(buffer)-1);
        if (count >= 0) {
            buffer[count] = 0;
            printf("%s", buffer);
        } else {
            printf("IO Error\n");
        }
    }
}