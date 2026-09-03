#ifndef PROC_INFO_H
#define PROC_INFO_H

// сама структура типа - pid и parent pid
// название процесса и состояние
// командная строка и количество открытых файловых дескрипторов
typedef struct
{
    unsigned int pid;
    unsigned int parent_pid;
    char name[256];
    char state[32];
    char *command_line;
    unsigned int fd_count;
} ProcessInfo;

// функция безопасной инициализации перед использованием
void process_info_init(ProcessInfo *info);
// функция осовобождения динамической памяти структуры
void process_info_destroy(ProcessInfo *info);

#endif