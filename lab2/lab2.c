#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

int main() {
    // Открытие директории "input"
    DIR *dir = opendir("input");
    struct dirent *entry;
    if (dir == NULL) {
        perror("Failed to open directory");
        return 1;
    }

    // Открытие файла для вывода результата в "output/output.txt"
    FILE *output_file = fopen("output/output2.txt", "w");
    if (output_file == NULL) {
        perror("Failed to open output file");
        return 1;
    }

    char max_file_name[100];
    int max_space_count = -1;  // Используйте -1 как начальное значение

    // Цикл по всем элементам в директории "input"
    while ((entry = readdir(dir)) != NULL) {
        // Проверка, является ли текущий элемент обычным файлом
        if (entry->d_type == DT_REG) { // Check if entry is a regular file
            char input_path[100];
            // Формирование полного пути к входному файлу
            sprintf(input_path, "input/%s", entry->d_name); // Construct input file path

            // Открытие входного файла для чтения
            FILE *input_file = fopen(input_path, "r");
            if (input_file == NULL) {
                perror("Failed to open input file");
                return 1;
            }

            int space_count = 0;
            int c;
            // Подсчет количества пробелов в файле
            while ((c = fgetc(input_file)) != EOF) {
                if (c == ' ') {
                    space_count++;
                }
            }

            // Проверка, является ли текущий файл файлом с максимальным количеством пробелов
            if (space_count > max_space_count) {
                max_space_count = space_count;
                strcpy(max_file_name, entry->d_name);
            }

            char output_line[100];
            // Формирование строки вывода с именем файла и количеством пробелов
            sprintf(output_line, "%s: %d spaces\n", entry->d_name, space_count); // Construct output line
            // Запись строки в выходной файл
            fprintf(output_file, "%s", output_line);
            // Вывод имени файла в консоль
            printf("%s\n", entry->d_name);

            // Закрытие входного файла
            fclose(input_file);
        }
    }
    fprintf(output_file, "File with max spaces: %s\n", max_file_name);
    // Вывод имени файла с максимальным количеством пробелов
    printf("File with max spaces: %s\n", max_file_name);

    // Закрытие директории и выходного файла
    closedir(dir);
    fclose(output_file);
    // Вывод сообщения об успешном завершении
    printf("Output written to output/output2.txt\n");

    return 0;
}

