#include "args.h"
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

// args.h - наш заголовок
// errno.h - для проверки ошибки преобразования через strlot или strtoul
// limits.h - для ограничения типов как пример UINT_MAX
// stdlib.h - тут типа объявлена сама функция strlot/


int parse_pid(const char *text, unsigned int *pid){
    // ну типа strtol возвращает через этот указатель место,
    // на котором остановилось преобразование
    char *end;
    // временная переменная для числа полученного из строки
    long value;

    // обнуляем чтобы стртол проверил произошла ли ошибка преобразования
    errno = 0;
    // вызов преобразования с системой счисления 10
    value = strtol(text, &end, 10);

    // проверка переполнения при преобразовании и вмещается ли в лонг
    if (errno == ERANGE || value > UINT_MAX)
    {
        return 1;
    }

    // проверка где закончилось преобразование
    // тип если текст = абс то энд тоже абс
    if (end == text)
    {
        return 1;
    }
    // проверка на числа лишние
    if (*end != '\0')
    {
        return 1;
    }
    // проверка что pid больше 0
    if (value <= 0)
    {
        return 1;
    }

    *pid = (unsigned int)value;

    return 0;

}