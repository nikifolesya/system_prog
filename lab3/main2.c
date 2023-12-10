#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

/*Напишите программу, которая выполняет обработку данных согласно варианту из лабораторной работы №2, 
но делает это, используя две нити – одна читает информацию из входных файлов и обрабатывает её, а другая записывает результат в файл.*/

// Максимальная длина имени файла
#define MAX_FILENAME_LENGTH 100

// Глобальные переменные для хранения максимального количества пробелов и имени файла
int max_space_count = -1;
char max_file_name[MAX_FILENAME_LENGTH];

// Мьютекс для синхронизации доступа к глобальным переменным
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Функция обработки данных (читает информацию из входных файлов и обрабатывает её)
void *processData(void *arg) {
    // Открываем директорию "input"
    DIR *dir = opendir("input");
    struct dirent *entry;

    // Перебираем все файлы в директории
    while ((entry = readdir(dir)) != NULL) {
        // Проверяем, является ли текущий элемент файлом (а не директорией или другим объектом)
        if (entry->d_type == DT_REG) {
            // Формируем полный путь к файлу
            char input_path[MAX_FILENAME_LENGTH];
            sprintf(input_path, "input/%s", entry->d_name);

            // Открываем входной файл для чтения
            FILE *input_file = fopen(input_path, "r");
            if (input_file == NULL) {
                perror("Failed to open input file");
                pthread_exit(NULL);
            }

            int space_count = 0;
            int c;

            // Подсчитываем количество пробелов в файле
            while ((c = fgetc(input_file)) != EOF) {
                if (c == ' ') {
                    space_count++;
                }
            }

            fclose(input_file);

            // Захватываем мьютекс для доступа к глобальным переменным
            pthread_mutex_lock(&mutex);

            // Обновляем максимальное количество пробелов и имя файла, если необходимо
            if (space_count > max_space_count) {
                max_space_count = space_count;
                strcpy(max_file_name, entry->d_name);
            }

            // Открываем выходной файл для записи результата
            FILE *output_file = fopen("output/output2.txt", "a");
            if (output_file == NULL) {
                perror("Failed to open output file");
                pthread_mutex_unlock(&mutex);
                pthread_exit(NULL);
            }

            // Записываем информацию о количестве пробелов в выходной файл и выводим имя файла в консоль
            fprintf(output_file, "%s: %d spaces\n", entry->d_name, space_count);
            printf("%s\n", entry->d_name);

            fclose(output_file);

            // Освобождаем мьютекс после доступа к глобальным переменным
            pthread_mutex_unlock(&mutex);
        }
    }

    // Закрываем директорию "input"
    closedir(dir);

    // Завершаем поток
    pthread_exit(NULL);
}

// Функция записи результата в файл
void *writeResult(void *arg) {
    // Получаем идентификатор текущего потока
    // pthread_t mythid;
    // mythid = pthread_self();

    // Открываем выходной файл для записи результата
    FILE *output_file = fopen("output/output2.txt", "a");
    if (output_file == NULL) {
        perror("Failed to open output file");
        pthread_exit(NULL);
    }

    // Захватываем мьютекс для доступа к глобальным переменным
    pthread_mutex_lock(&mutex);

    // Записываем в выходной файл информацию о файле с максимальным количеством пробелов и выводим его имя в консоль
    fprintf(output_file, "File with max spaces: %s\n", max_file_name);
    printf("File with max spaces: %s\n", max_file_name);

    // Освобождаем мьютекс после доступа к глобальным переменным
    pthread_mutex_unlock(&mutex);

    // Закрываем выходной файл
    fclose(output_file);

    // Завершаем поток
    pthread_exit(NULL);
}

// Основная функция программы
int main() {
    // Объявление переменных для идентификаторов потоков и результатов создания потоков
    pthread_t thread1, thread2;
    int result1, result2;

    // Создаем первый поток для обработки данных
    result1 = pthread_create(&thread1, NULL, processData, NULL);
    if (result1 != 0) {
        perror("Error on thread1 create");
        exit(EXIT_FAILURE);
    }

    // Создаем второй поток для записи результата
    result2 = pthread_create(&thread2, NULL, writeResult, NULL);
    if (result2 != 0) {
        perror("Error on thread2 create");
        exit(EXIT_FAILURE);
    }

    // Ожидаем завершения обоих потоков
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Выводим сообщение об успешном завершении
    printf("Output written to output/output2.txt\n");

    // Завершаем программу
    return 0;
}

