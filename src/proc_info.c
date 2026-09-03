#include "proc_info.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

// буфер для хранения строки статуса командной строки и колво дестрипторов
#define PATH_SIZE 64

// proc_info.h - заголовок
// stddef.h - для NULL
// stdlib.h - тут типа объявлена 
// stdio.h для snprintf

// безопасная инициализация всех полей структуры
void process_info_init(ProcessInfo *info)
{
    info->pid = 0;
    info->parent_pid = 0;
    info->name[0] = '\0';
    info->state[0] = '\0';
    info->command_line = NULL;
    info->fd_count = 0;
}

// освобождение динамической памяти структуры и приведение
// в безопасное состояние перед завершением работы
void process_info_destroy(ProcessInfo *info)
{
    // освобождает память которуб программа
    // потом выделит под командной строки процесса
    free(info->command_line);
    // ну указатель нал и чё дальше
    info->command_line = NULL;  
}

// так как snprintf всё равно должен куда либо записывать результат
// то буферы нужны как обычные локальные массивы для путей
// поэтому и создана переменная размера буфера в 64 байта

// функция которая юзается main.c для получения инфы о процессе через pid
int process_info_read(ProcessInfo *info, unsigned int pid)
{
    // три локальных буфера для трёх путей аналогично
    char status_path[PATH_SIZE];
    char cmdline_path[PATH_SIZE];
    char fd_path[PATH_SIZE];

    // тут будет результат работы snprintf
    int result;


    // для status
    result = snprintf(status_path, PATH_SIZE, "/proc/%u/status", pid);

    // проверка произошла ли ошибка или строка не поместилась 
    // полностью в бфуер и была обрезана
    if (result < 0 || result >= PATH_SIZE)
    {
        return 1;
    }


    // для cmdline
    result = snprintf(cmdline_path, PATH_SIZE, "/proc/%u/cmdline", pid);

    // проверка произошла ли ошибка или строка не поместилась 
    // полностью в бфуер и была обрезана
    if (result < 0 || result >= PATH_SIZE)
    {
        return 1;
    }


    // для fd
    result = snprintf(fd_path, PATH_SIZE, "/proc/%u/fd", pid);

    // проверка произошла ли ошибка или строка не поместилась 
    // полностью в бфуер и была обрезана
    if (result < 0 || result >= PATH_SIZE)
    {
        return 1;
    }

}