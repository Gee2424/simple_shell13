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
    char *path = "/bin"; /* PATH directory */
    char command_path[MAX_INPUT_LENGTH];

    while (1)
    {
        printf(":) "); /* Display the shell prompt */

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

            snprintf(command_path, sizeof(command_path), "%s/%s", path, args[0]);

            execve(command_path, args, NULL); /* Execute the command */
            exit(127); /* Exit with an error code if execve fails */
        }
        else if (pid > 0) /* Parent process */
        {
            int status;
            wait(&status); /* Wait for the child process to finish */
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
