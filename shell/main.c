#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <pwd.h>
#include "include/shell.h"


int main() {
    char *line;  // User input
    char **args; // Tokens in user input
    int status;  // Status of execution

    // Add signal for killing foreground child on ctrl-c
    signal(SIGINT, kill_foreground);
    // Add signal for handling end of child processes
    signal(SIGCHLD, mark_ended_task);

    do {
        // Printing left shell info
        display();

        // Reading user input
        line = readline();
        if (line == NULL) {
            exit(1);
        }

        // Parse line to tokens
        args = split(line);
        if (args == NULL) {
            free(line);
            exit(2);
        }

        // Try execute command
        status = execute(args);

        // Free allocated memory
        free(line);
        free(args);

    } while (status);

    return 0;
}
