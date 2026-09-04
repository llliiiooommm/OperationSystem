#include "args.h"
#include "proc_info.h"
#include "output.h"

#include <stdio.h>

// ну в main нужно объединить и как то юзнуть. буду пробовать
int main(int argc, char *argv[])
{
    // тип должно получать один аргумент после имени программы 
    // (тип ./bin/procview 1234)
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <PID>\n", argv[0]);
        return 1;
    }

    // работа с parse_pid
    // создаётся переменная для числового pid
    unsigned int pid;
    // из командной строки argv[1] = "1234" превращает в строку "1234"
    // в число 1234 и записывает через указатель pid
    // и если parse_pid вернёт не 0 значит pid некорректный.
    if (parse_pid(argv[1], &pid) != 0)
    {
        fprintf(stderr, "Invalid PID: %s\n", argv[1]);
        return 1;
    }

    // создаём переменную структуры которая определена в proc_info.h
    ProcessInfo info;
    // вызываем функцию инициализации
    process_info_init(&info);

    // тут уже процесс работы с директориями
    // в process_info_destroy работа с освобождением динамической памяью, 
    // которая могла успеть выделиться до ошибки
    if (process_info_read(&info, pid) != 0)
    {
        process_info_destroy(&info);
        return 1;
    }

    // передает заполненную структуру в файл вывода
    output_process_info(&info);

    // процесс очищения динамической памяти которая была выделена
    process_info_destroy(&info);

    return 0;
}