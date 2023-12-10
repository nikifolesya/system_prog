#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char *argv[]) {
    DIR *dir;                     // Дескриптор директории
    struct dirent *entry;         // Структура для представления записей в директории
    struct stat inode_stat;       // Структура для хранения информации о файле (inode)
    int max_links = 0;            // Максимальное количество жестких ссылок на файл
    ino_t max_inode;              // Номер inode с максимальным количеством ссылок
    char *max_files[100];         // Массив имен файлов с максимальным количеством ссылок
    int num_files = 0;            // Общее количество файлов с максимальным количеством ссылок

    // Проверка наличия аргумента командной строки (пути к директории)
    if (argc != 2) {
        printf("Usage: %s directory\n", argv[0]);
        return 1;
    }

    // Открытие директории для чтения
    dir = opendir(argv[1]);
    if (dir == NULL) {
        printf("Error opening directory %s\n", argv[1]);
        return 1;
    }

    // Обход файлов в директории
    while ((entry = readdir(dir)) != NULL) {
        char path[1024];
        // Составление полного пути к файлу
        sprintf(path, "%s/%s", argv[1], entry->d_name);

        // Получение информации о файле (inode)
        if (stat(path, &inode_stat) == -1) {
            printf("Error getting inode for %s\n", path);
            continue;
        }

        // Проверка, является ли файл обычным файлом (не каталогом, не символической ссылкой и т. д.)
        if (S_ISREG(inode_stat.st_mode)) {
            // Если количество жестких ссылок больше текущего максимума
            if (inode_stat.st_nlink > max_links) {
                max_links = inode_stat.st_nlink;
                max_inode = inode_stat.st_ino;
                num_files = 0;
                max_files[num_files++] = strdup(entry->d_name);
            } 
            // Если количество жестких ссылок равно текущему максимуму
            else if (inode_stat.st_nlink == max_links) {
                max_files[num_files++] = strdup(entry->d_name);
            }
        }
    }

    // Вывод информации о файле с максимальным количеством ссылок
    printf("Max inode with %d links: %llu\n", max_links, max_inode);
    printf("Files: ");
    // Вывод имен файлов с максимальным количеством ссылок и освобождение выделенной памяти
    for (int i = 0; i < num_files; i++) {
        printf("%s ", max_files[i]);
        free(max_files[i]);
    }
    printf("\n");

    // Закрытие директории
    closedir(dir);
    return 0;
}
