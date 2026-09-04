#include "output.h"
#include "stdio.h"

// ну реализация функции вывода
void output_process_info(const ProcessInfo *info)
{
    printf("Process information\n");
    // %u т к pid имеет unsigned int
    printf("PID: %u\n", info->pid);
    // %s т к name строка
    printf("Name: %s\n", info->name);
    // %s т к state строка
    printf("State: %s\n", info->state);
    // %u т к parent_pid имеет тип unsigned int
    printf("Parent PID: %u\n", info->parent_pid);

    // проверка существования строки - если существует - выводим
    // иначе строка из методички
    if (info->command_line != NULL)
    {
        printf("Command line: %s\n", info->command_line);
    }
    else
    {
        printf("Command line: [not available]\n");
    }

    // колво открытых файловых дескрипторов - также тип fd_cound является
    // unsigned int
    printf("Open file descriptors: %u\n", info->fd_count);

}

