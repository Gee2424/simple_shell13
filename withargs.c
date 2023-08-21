#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_LENGTH 1024

int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    pid_t pid;

    while (1)
    {
        printf("#cisfun$ "); /* Display the shell prompt */

        read = getline(&line, &len, stdin); /* Read user input */

        if (read == -1) /* Handle Ctrl+D (EOF) */
        {
            printf("\n");
            break;
        }

        line[read - 1] = '\0'; /* Remove the newline character */

        if (strcmp(line, "exit") == 0) /* Exit the shell */
        {
            free(line);
            exit(0);
        }

        pid = fork(); /* Create a child process */

        if (pid == 0) /* Child process */
        {
            char *args[MAX_INPUT_LENGTH];
            int i = 0;
            char *token = strtok(line, " "); /* Tokenize input */

            while (token != NULL)
            {
                args[i++] = token;
                token = strtok(NULL, " ");
            }
            args[i] = NULL; /* Null-terminate the argument array */

            execvp(args[0], args); /* Execute the command */
            perror("Execution error"); /* Exec only returns if there's an error */
            exit(1);
        }
        else if (pid > 0) /* Parent process */
        {
            wait(NULL); /* Wait for the child process to finish */
        }
        else
        {
            perror("Fork error");
            exit(1);
        }
    }

    free(line);
    return (0);
}
