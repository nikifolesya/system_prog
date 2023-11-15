#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>
#include <signal.h>

#define DEFAULT_BUFF_SIZE 16
#define TOKENS_DELIMITERS " \t"

#define CONTINUE 1
#define EXIT 0

#define PRIMARY_COLOR "\033[92m"
#define SECONDARY_COLOR "\033[90m"
#define RESET_COLOR "\033[0m"

struct bg_task_t
{
    pid_t pid;       // Process id
    bool finished;   // Process state
    char *timestamp; // Process state
    char *cmd;       // Command cmd
};
typedef struct bg_task_t bg_task;

struct fg_task_t
{
    pid_t pid;     // Process id
    bool finished; // Process state
};
typedef struct fg_task_t fg_task;

struct tasks_t
{
    fg_task foreground;  // Process id of foreground bg_task
    bg_task *background; // Background task list
    size_t cursor;       // Cursor of background tasks
    size_t capacity;     // Background array capacity
};
typedef struct tasks_t tasks;

char **split(char *line) {
    size_t position = 0;
    size_t buff_size = DEFAULT_BUFF_SIZE;

    char *token;

    // Allocate memory for tokens array
    char **tokens = (char **)malloc(sizeof(char *) * buff_size);
    if (tokens == NULL) {
        printf("[ERROR] Couldn't allocate buffer for splitting!\n");
        return NULL;
    }

    // Tokenize process
    token = strtok(line, TOKENS_DELIMITERS);
    while (token != NULL) {
        // Emplace token to array
        tokens[position++] = token;

        // If array free space ended - increase array
        if (position >= buff_size) {
            buff_size *= 2;
            tokens = (char **)realloc(tokens, buff_size * sizeof(char *));
            if (tokens == NULL) {
                printf("[ERROR] Couldn't reallocate buffer for tokens!\n");
                return NULL;
            }
        }

        // Getting next token
        token = strtok(NULL, TOKENS_DELIMITERS);
    }

    // Place NULL to the end of tokens array
    tokens[position] = NULL;

    return tokens;
}

char *readline()
{
    char *line = NULL;
    size_t size = 0;
    ssize_t str_len;

    // Reading line from stdin
    if ((str_len = getline(&line, &size, stdin)) == -1)
    {
        // Logging all errors except Ctrl-D - terminal shutdown
        if (errno != 0)
        {
            printf("[ERROR] Couldn't read from stdin\n");
        }
        free(line);
        printf("\n");
        return NULL;
    }

    // Remove useless \n symbol if exists
    if (line[str_len - 1] == '\n')
    {
        line[str_len - 1] = '\0';
    }

    return line;
}

tasks t = {
    .foreground = {
        .pid = -1,
        .finished = true},
    .background = NULL,
    .cursor = 0,
    .capacity = 0

};

void set_foreground(pid_t pid) {
    t.foreground.pid = pid;
    t.foreground.finished = 0;
}

int add_background(pid_t pid, char *name) {
    // Temp background task variable
    bg_task *bt;

    // If end of free space in background array - increase size
    if (t.cursor >= t.capacity) {
        t.capacity = t.capacity * 2 + 1;
        t.background = (bg_task *)realloc(t.background, sizeof(bg_task) * t.capacity);
        if (t.background == NULL) {
            printf("[ERROR] Couldn't reallocate buffer for background tasks!\n");
            return -1;
        }
    }

    // Print info about process start
    printf("[%zu] started.\n", t.cursor);

    // Save task in temp variable
    bt = &t.background[t.cursor];
    // Save process info in array
    bt->pid = pid;
    bt->finished = false;

    time_t timestamp = time(NULL);
    bt->timestamp = ctime(&timestamp);

    bt->cmd = strdup(name);

    // Move cursor right
    t.cursor += 1;

    return 0;
}

void kill_foreground() {
    if (t.foreground.pid != -1) {
        // Kill process
        kill(t.foreground.pid, SIGTERM);

        // Set finished flag
        t.foreground.finished = true;

        printf("\n");
    }
}

int term(char **args) {
    char *idx_str;    // Cursor in index arg
    int proc_idx = 0; // Converted to int index arg


    if (args[1] == NULL) {
        printf("[ERROR] No process index to stop!\n");
    }
    else {
        // Set cursor in index arg
        idx_str = args[1];

        // Convert string index arg to int
        while (*idx_str >= '0' && *idx_str <= '9') {
            proc_idx = (proc_idx * 10) + ((*idx_str) - '0');

            // Move cursor to right
            idx_str += 1;
        }

        // Kill process if process index not bad
        // and target process not finished
        if (*idx_str != '\0' || proc_idx >= t.cursor) {
            printf("[ERROR] Incorrect background process index!\n");
        }
        else if (!t.background[proc_idx].finished) {
            kill(t.background[proc_idx].pid, SIGTERM);
        }
    }

    return CONTINUE;
}

int is_background(char **args) {
    // Current position in array
    int last_arg = 0;

    // Finding last arg in array
    while (args[last_arg + 1] != NULL) {
        last_arg += 1;
    }

    // Checking if task is background`
    if (strcmp(args[last_arg], "&") == 0) {
        // Remove '&' token for future executing
        args[last_arg] = NULL;

        // Return true
        return 1;
    }

    // Return false if: '&' wasn't founded
    return 0;
}

int quit() {
    // Temp background task variable
    bg_task *bt;

    // Disable logging on child killed
    signal(SIGCHLD, SIG_IGN);

    // Kill foreground process
    if (!t.foreground.finished) {
        kill_foreground();
    }

    // Kill all active background tasks
    for (size_t i = 0; i < t.cursor; i++) {
        // Place background task to temp variable
        bt = &t.background[i];

        // Kill process if active
        if (!bt->finished)
        {
            kill(bt->pid, SIGTERM);
        }

        // Free memory for command name
        free(bt->cmd);
    }

    return EXIT;
}

int launch(char **args) {
    pid_t pid;      // Fork process id
    int background; // Is background task
    // Checking if task is background
    background = is_background(args);
    // Create child process
    pid = fork();
    // If created failure log error
    if (pid < 0) {
        printf("[ERROR] Couldn't create child process!\n");
    }
    // Child process
    else if (pid == 0) {
        // Try launch task
        if (execvp(args[0], args) == -1)
        {
            printf("[ERROR] Couldn't execute unknown command!\n");
        }
        exit(1);
    }
    // Parent process
    else {
        if (background) {
            // Try add background task to array
            if (add_background(pid, args[0]) == -1) {
                // Kill all processes and free
                // memory before exit
                quit();
            }
        }
        else {
            // Set foreground task to store
            set_foreground(pid);

            // Wait while process not ended
            if (waitpid(pid, NULL, 0) == -1) {
                // Logging error if process tracked with error
                // Except when interrupted by a signal
                if (errno != EINTR) {
                    printf("[ERROR] Couldn't track the completion of the process!\n");
                }
            }
        }
    }
    return CONTINUE;
}


int cd(char **args) {
    if (args[1] == NULL) {
        printf("[ERROR] Expected argument for \"cd\" command!\n");
    }
    else if (chdir(args[1]) != 0) {
        printf("[ERROR] Couldn't change directory to \"%s\"!\n", args[1]);
    }
    return CONTINUE;
}

int help() {
    printf(
        "Simple shell by Denis Glazkov,                               \n\n"

        "Just type program names and arguments, and hit enter.          \n"
        "Run tasks in background using '&' in the end of command.     \n\n"


        "Built in functions:                                           \n"
        "  cd   <path>        - Changes current working directory      \n"
        "  term <bg_task_idx> - Kill background process by index       \n"
        "  help               - Prints info about shell                \n"
        "  bg                 - Prints list of background tasks        \n"
        "  quit               - Terminates shell and all active tasks\n\n"

        "Use the man command for information on other programs.         \n");

    return CONTINUE;
}

int bg() {
    // Temp background task variable
    bg_task *bt;

    for (size_t i = 0; i < t.cursor; i++) {
        // Store background task in temp variable
        bt = &t.background[i];
        // Print info about task
        printf(
            "[%zu]%s cmd: %s%s;%s pid: %s%d; %s"
            "state: %s%s;%s timestamp: %s%s",
            i,
            SECONDARY_COLOR, RESET_COLOR, bt->cmd,
            SECONDARY_COLOR, RESET_COLOR, bt->pid,
            SECONDARY_COLOR, RESET_COLOR, bt->finished ? "finished" : "active",
            SECONDARY_COLOR, RESET_COLOR, bt->timestamp);
    }
    return CONTINUE;
}

void mark_ended_task()
{
    // Temp background task variable
    bg_task *bt;

    // Get process id of ended process
    pid_t pid = waitpid(-1, NULL, 0);

    // Handle foreground process
    if (pid == t.foreground.pid)
    {
        t.foreground.finished = true;
    }
    // Handle background process
    else
    {
        // Search and remove process form background tasks array
        for (size_t i = 0; i < t.cursor; i++)
        {
            // Place task to temp variable
            bt = &t.background[i];

            if (bt->pid == pid)
            {
                // Print info about process end
                printf("[%zu] finished.\n", i);

                // Set new state for background process
                bt->finished = 1;

                break;
            }
        }
    }
}

int execute(char **args)
{
    if (args[0] == NULL)
    {
        return CONTINUE;
    }
    else if (strcmp(args[0], "cd") == 0)
    {
        return cd(args);
    }
    else if (strcmp(args[0], "help") == 0)
    {
        return help();
    }
    else if (strcmp(args[0], "quit") == 0)
    {
        return quit();
    }
    else if (strcmp(args[0], "bg") == 0)
    {
        return bg();
    }
    else if (strcmp(args[0], "term") == 0)
    {
        return term(args);
    }
    else
    {
        return launch(args);
    }
}

void display()
{
    // Try get and print username with color
    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);
    if (pw != NULL)
    {
        printf("%s%s%s:", PRIMARY_COLOR, pw->pw_name, RESET_COLOR);
    }

    // Try get and print current directory with color
    char cwd[100];
    if (getcwd(cwd, 100) != NULL)
    {
        printf("%s%s%s", SECONDARY_COLOR, cwd, RESET_COLOR);
    }

    // Print end of shell info
    printf("# ");
}