#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {

    pid_t pid;
    int status;

    printf("=====================================\n");
    printf("       CO-2 EXEC PROCESS DEMO\n");
    printf("=====================================\n");

    printf("\nParent Process ID: %d\n", getpid());

    // Create child process
    pid = fork();

    if (pid < 0) {

        perror("fork failed");
        return 1;
    }

    // Child process
    else if (pid == 0) {

        printf("\n-------------------------------------\n");
        printf("        CHILD PROCESS\n");
        printf("-------------------------------------\n");

        printf("Child PID: %d\n", getpid());

        printf("\nChild will execute:\n");
        printf("ls -l backup\n");

        printf("\nExecuting command...\n\n");

        // Replace child process with ls command
        execlp(
            "ls",
            "ls",
            "-l",
            "backup",
            NULL
        );

        // This executes only if exec fails
        perror("exec failed");

        exit(1);
    }

    // Parent process
    else {

        printf("\n-------------------------------------\n");
        printf("        PARENT PROCESS\n");
        printf("-------------------------------------\n");

        printf("Parent is waiting for child...\n");

        wait(&status);

        printf("\nChild execution completed.\n");

        if (WIFEXITED(status)) {

            printf("Child exit status: %d\n",
                   WEXITSTATUS(status));
        }
    }

    printf("\nParent process finished.\n");

    return 0;
}