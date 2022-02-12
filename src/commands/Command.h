#ifndef ARCHITECTURY_COMMAND_H
#define ARCHITECTURY_COMMAND_H

#include <cstdint>
#include "../common.h"

class CPU; // для избежания циклического включения Command.h -> CPU.h -> Command.h ...

/**
 * @brief базовый класс команды процессора.
 * @details
 * формат команды:\n
 * p opcode s r1 r2 offset, где\n
 *\n
 * p - префикс: t - tiny (короткая команда), 2 байта\n
 * 				b - big (длинная команда), 4 байта\n
 *\n
 * opcode - числовой код команды\n
 *\n
 * s - размер операнда: 0 - короткое целое число, 2 байта\n
 * 						1 - вещественное или длинное целое число, 4 байта\n
 *\n
 * r1 - регистр первого операнда\n
 * r2 - регистр второго операнда\n
 * offset - короткое беззнаковое число, используется и обрабатывается только в длинных командах
 */
struct Command
{
	virtual void operator()(CPU & cpu) = 0;
};

#endif //ARCHITECTURY_COMMAND_H
