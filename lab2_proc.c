#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // функции для работы с операционной системой 
#include <sys/wait.h> //функции для управления процессами и ожидания их завершения. 

int main() {
    pid_t pid1, pid2, pid, ppid;
    int status = 0; // Инициализируем переменную status

    pid1 = fork(); // Создаем процесс-потомок 1

    if (pid1 < 0) { // Проверяем на ошибку создания процесса
        fprintf(stderr, "Fork failed\n");
        exit(1);
    } else if (pid1 == 0) { // Код для процесса-потомка 1
        // Child process 1
        status = status + 1;
        printf("Child process 1 is executing lab1.c\n");
        execlp("./lab1", "lab1", NULL);
        // Если execlp() выполнится успешно, код ниже этой строки не выполнится

    } else { // Код для родительского процесса
        // Parent process
        status = status + 1;
        waitpid(pid1, &status, 0); // Ждем завершения процесса-потомка 1 и получаем статус
        printf("Child process 1 has finished\n");
        pid = getpid();
        ppid = getppid();
        printf("My pid = %d, my ppid = %d, result = %d\n", (int)pid, (int)ppid, status);
        pid2 = fork(); // Создаем процесс-потомок 2

        if (pid2 < 0) { // Проверяем на ошибку создания процесса
            fprintf(stderr, "Fork failed\n");
            exit(1);
        } else if (pid2 == 0) { // Код для процесса-потомка 2
            // Child process 2
            status = status + 1;
            printf("Child process 2 is executing lab2.c\n");
            execlp("./lab2", "lab2", NULL);
            // Если execlp() выполнится успешно, код ниже этой строки не выполнится

        } else { // Код для родительского процесса
            // Parent process
            status = status + 1;
            waitpid(pid2, &status, 0); // Ждем завершения процесса-потомка 2 и получаем статус
            printf("Child process 2 has finished\n");
        }
    }

    pid = getpid();
    ppid = getppid();
    printf("My pid = %d, my ppid = %d, result = %d\n", (int)pid, (int)ppid, status);
    return 0;
}
