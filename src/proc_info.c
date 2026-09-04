#include "proc_info.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>

// буфер для хранения строки статуса командной строки и колво дестрипторов
#define PATH_SIZE 64

// proc_info.h - заголовок
// stddef.h - для NULL
// stdlib.h - тут типа объявлена 
// stdio.h для snprintf

// dirent.h нужен для работы с каталогами и функциями 
// так как функции opendir readdir closedir указано в задании

// errno.h нужно для понимания была ли ошибка во время чтения каталога

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


    // ================================================================
    //                      STATUS
    // ================================================================
    // открывает status_path для чтения
    FILE *file;
    file = fopen(status_path, "r");

    if (file == NULL)
    {
        // тип если не смог открыть каталог /proc/<pid>/status, 
        // то выведет системную ошибку в stderr
        perror(status_path);
        return 1;
    }

    char line[256];

    // флаги наличия обязательных полей
    int name_found = 0;
    int state_found = 0;
    int pid_found = 0;
    int ppid_found = 0;

    // тип продолжаем читать пока fgets получает очередную строку
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strncmp(line, "Name:", 5) == 0)
        {
            // тип name занимает 5 символов так что после должно указываться
            char *value = line + 5;

            // пропуск пробелов и табуляций
            while (*value == ' ' || *value == '\t')
            {
                value++;
            }

            // удаление перевода строки \n который fgets изначально оставил в конце
            value[strcspn(value, "\n")] = '\0';

            // копируется полученное имя в info->name и гарантируется, 
            // что будет наличие завершающего '\0'
            strncpy(info->name, value, sizeof(info->name) - 1);
            info->name[sizeof(info->name) - 1] = '\0';

            name_found = 1;
        }

        if (strncmp(line, "State:", 6) == 0)
        {
            // тип value занимает 6 символов так что после должно указываться
            char *value = line + 6;

            // пропуск пробелов и табуляций
            while (*value == ' ' || *value == '\t')
            {
                value++;
            }

            // удаление перевода строки \n который fgets изначально оставил в конце
            value[strcspn(value, "\n")] = '\0';

            // копируется полученное состояние в info->state и гарантируется, 
            // что будет наличие завершающего '\0'
            strncpy(info->state, value, sizeof(info->state) - 1);
            info->state[sizeof(info->state) - 1] = '\0';

            state_found = 1;
        }

        // проверяет начинается ли текущая строка с pid
        if (strncmp(line, "Pid:", 4) == 0)
        {
            // передает в strtoul часть строки после pid и преобразует текст в число
            info->pid = (unsigned int)strtoul(line + 4, NULL, 10);
            pid_found = 1;
        }

        // проверяет начинается ли текущая строка с Ppid
        if (strncmp(line, "PPid:", 5) == 0)
        {
            // передает в strtoul часть строки после ppid и преобразует текст в число
            info->parent_pid = (unsigned int)strtoul(line + 5, NULL, 10);
            ppid_found = 1;
        }
    }

    // если хотя бы один равен 0 значет обязательное полве в status не найдено
    if (!name_found || !state_found || !pid_found || !ppid_found)
    {
        fclose(file);
        return 1;
    }

    fclose(file);



    // ================================================================
    //                      CMDLINE
    // ================================================================
    // открывает cmdline_path для чтения
    file = fopen(cmdline_path, "rb");

    if (file == NULL)
    {
        // тип если не смог открыть /proc/<pid>/cmdline,
        // то даст ошибку в stderr
        perror(cmdline_path);
        return 1;
    }

    // указатель на динамическую строку в которую собрать 
    // надо командную строку процесса - начинается с null
    char *command_line = NULL;
    // хранит текущий размер выделенного буфера
    size_t command_size = 0;

    // далее будет считываться cmdline по байтово и 
    // увеличиваться буфер если надо

    // переменная хранящая очередной считанный байт
    int character;

    // читает по одному байту пока не достигнет конца файла
    while ((character = fgetc(file)) != EOF)
    {
        // необходим временный указатель, так как если realloc не сможет
        // выделить память то старый указатель command_line ещё понадобится для free 
        char *new_command_line;

        // т к command_line это динамический буфер, 
        // а command_size это колво символов в command_line,
        // то при первом символе необходимо место для 'символ' + '\0', 
        // то есть command_size
        new_command_line = realloc(command_line, command_size + 2);

        // это проверка при ошибке выделения
        if (new_command_line == NULL)
        {
            free(command_line);
            fclose(file);
            return 1;
        }

        command_line = new_command_line;

        // если прочитал нулевой байт то ' '
        if (character == '\0')
        {
            command_line[command_size] = ' ';
        }
        else
        {
            command_line[command_size] = (char)character;
        }

        // после добавления символа увеличивается колво записанных символов
        command_size++;
        // и в каждый момент оставляет строку с завершающим нулевым байтом '\0'
        command_line[command_size] = '\0';
    }

    // важная информация - цикл завершается в двух случаях:
    // 1) действительно достигнут конец файла
    // 2) при чтении произошла ошибка
    // ferror(file) ниже проверяет именно второй случай
    // ну типа если ошибка произошла то освобождается уже выделенная память
    if (ferror(file))
    {
        free(command_line);
        fclose(file);
        return 1;
    }

    // убираются лишние '\0' и послендний '\0' заменяется в пробел
    if (command_size > 0 && command_line[command_size - 1] == ' ')
    {
        command_line[command_size - 1] = '\0';
        command_size--;
    }

    // закрывается cmdline и передаётся собранная 
    // динамическая строка в наш processinfo
    fclose(file);
    info->command_line = command_line;



    // ================================================================
    //                      FD
    // ================================================================
    // создаётся указатель на открытый каталог
    DIR *directory;

    // откурывается /proc/<pid>/fd
    directory = opendir(fd_path);

    // проверка открыточти каталога - если не удалось то ошибка
    if (directory == NULL)
    {
        // тип если не смог открыть /proc/<pid>/fd,
        // то даст ошибку в stderr
        perror(fd_path);
        return 1;
    }

    // указатель на текущую запись каталога которую возвращает readdir
    // типа через указатель entry получают очередную запись каталога
    struct dirent *entry;

    // перед началом подсчёта устанавливаем счётчик в ноль
    info->fd_count = 0;

    // тут обнуляем для проверки ошибки
    errno = 0;

    // последовательно читает все записи каталога
    while ((entry = readdir(directory)) != NULL)
    {
        // пропуск специальных знаков (тип .. и . т к они не файловые дескрипторы)
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        // оставшаяся запись считается одним открытым файловым дескриптором
        info->fd_count++;
    }

    // у цикла два варианта:
    // 1) readdir дошёл до конца каталога -> errno остаётся 0
    // 2)  при чтении произошла ошибка -> errno становится ненулевым
    // тип при ошибке закрываем каталог
    if (errno != 0)
    {
        closedir(directory);
        return 1;
    }

    // закрывает каталог /proc/<pid>/fd
    closedir(directory);

    return 0;

}