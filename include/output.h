#ifndef OUTPUT_H
#define OUTPUT_H

// нужно для processinfo в функции output_process_info
#include "proc_info.h"
// сама функция вывода, 
// где функция будет получать указатель на уже заполненную структуру
void output_process_info(const ProcessInfo *info);

#endif