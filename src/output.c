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
    // пробую юзнуть сразу на выводе перевод буквенного кода
    printf("State: %c (%s)\n", info->state[0], state_description(info->state[0]));
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

// для перевода буквенного кода состояния в русское полное описание
static const char *state_description(char state)
{
    switch (state)
    {
        case 'R':
            return "Выполняется или готов к выполнению";
        case 'S':
            return "Прерываемый сон, обычно ожидание события";
        case 'D':
            return "Непрерываемое ожидание, часто ожидание I/O";
        case 'T':
            return "Остановлен сигналом или трассировщиком";
        case 't':
            return "Остановлен отладчиком при трассировке";
        case 'Z':
            return "Zombie: процесс завершён, но ещё не собран родителем";
        case 'I':
            return "Idle: простаивающий поток ядра";
        default:
            return "Неизвестное состояние";
    }
}