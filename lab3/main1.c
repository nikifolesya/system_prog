#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#include <string.h>


/*Напишите программу, в которой два процесса общаются через pipe, выполняя совместно обработку данных из лабораторной работы №1. 
Родительский процесс сначала читает информацию из входных файлов, производит необходимые вычисления, потом создаёт процесс потомок, 
передаёт ему через pipe результаты и ждёт окончания процесса-потомка. А процесс-потомок записывает результат в выходной файл.*/

int main() {
    int fd[2]; // массив для файловых дескрипторов ввода/вывода канала
    pid_t pid; // идентификатор процесса
    
    int space_count = 0; // переменная для подсчета пробелов
    
    // открываем директорию
    DIR *dir = opendir("input");
    if (dir == NULL) {
        perror("Failed to open directory");
        return 1;
    }
    
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) { // читаем, если директория не пустая
    
        if (pipe(fd) == -1) { // создаем канал
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        
        pid = fork(); // создаем новый процесс
            
        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        } 
        else if (pid > 0) {
            // Parent process
            
            close(fd[0]); // закрываем читающий конец канала
                
            if (entry->d_type == 8) { // проверка на регулярный файл
                char input_path[100]; // выделяем место под название входного файла
                sprintf(input_path, "input/%s", entry->d_name); // формируем полный путь к файлу

                FILE *input_file = fopen(input_path, "r"); // открываем файл для чтения
                if (input_file == NULL) {
                    perror("Failed to open input file");
                    return 1;
                }

                int c; // переменная для символов, поступающих на вход для проверки
                while ((c = fgetc(input_file)) != EOF) { // проверяем, дошли ли до конца, если нет - читаем дальше 
                    if (c == ' ') { // проверяем символ на пробел
                        space_count++;
                    }
                }
                
                fclose(input_file); // закрываем файл
            }
            
            // Write the result to the pipe
            write(fd[1], &space_count, sizeof(space_count));
            
            close(fd[1]); // закрываем пишущий конец канала
            
            wait(NULL); // ждем завершения дочернего процесса
                
        } else {
            // Child process
            
            close(fd[1]); // закрываем пишущий конец канала
            
            int spaces_count;
            
            // Read the result from the pipe
            read(fd[0], &spaces_count, sizeof(spaces_count));
            
            close(fd[0]); // закрываем читающий конец канала
            
            // Write the result to the output file
            FILE *output_file = fopen("output/output1.txt", "w");
            fprintf(output_file, "Number of spaces: %d\n", spaces_count);
            fclose(output_file);
            
            exit(EXIT_SUCCESS);
        }
    }
    
    closedir(dir); // закрываем директорию
    
    return 0;
}
