#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
// #include <iostream>
#include <stdlib.h>
  #include <fcntl.h>

#define BUFFER_SIZE 3000

// void	executeCGI(char **argv)
// {
// 	int fd[2];
// 	int	buf[BUFFER_SIZE];
// 	pipe(fd);
// 	char *envp[] = {NULL};
// 	int	pid = fork();
// 	int	defSTDOUT = dup(STDOUT_FILENO);


// 	if (pid == 0)
// 	{
	
// 		dup2(fd[1], STDOUT_FILENO);
// 		close(fd[1]);
// 		close(fd[0]);
// 		close(defSTDOUT);
// 		if (execve("/usr/bin/python3", argv, envp) == -1) {
// 			std::cout << "execve failed" << std::endl; }
				

//     }
// 	else{
// 	 close(fd[1]);
// 	dup2(defSTDOUT, STDOUT_FILENO);
// 	close(defSTDOUT);
// 	while (waitpid(-1, 0, 0))
// 		;
// 	while (read(fd[0], buf, BUFFER_SIZE) > 0)
// 	{
// 		write(2, "HAHAHHA", 8);
// 	}
// 	// 

// 	close(fd[0]);
// 	// close(fd[1]);
//     // This code will not be executed because execve replaces the process image
//     printf("This will not be printed\n");

// }
// }


int main() {
    int pipefd[2];
    pid_t pid;
    char buffer[1024];

    if (pipe(pipefd) == -1) {
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid == -1) {
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process

        // Close the read end of the pipe
        close(pipefd[0]);

        // Redirect stdout to the write end of the pipe
        dup2(pipefd[1], STDOUT_FILENO);

        // Execute the command
        char* args[] = { "ls", "-l", NULL };
        execvp(args[0], args);

        // If execvp returns, there was an error
    
        exit(EXIT_FAILURE);
    } else {
        // Parent process

        // Close the write end of the pipe
        close(pipefd[1]);

        // Open the output file for writing
        int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (fd == -1) {
    
            exit(EXIT_FAILURE);
        }

        // Read from the read end of the pipe and write to the output file
        int nbytes;
        while ((nbytes = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            if (write(fd, buffer, nbytes) == -1) {
             
                exit(EXIT_FAILURE);
            }
        }
        if (nbytes == -1) {
            
            exit(EXIT_FAILURE);
        }

        // Close the output file and wait for the child process to exit
        close(fd);
        int status;
        waitpid(pid, &status, 0);
    }

    return 0;
}