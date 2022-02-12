#ifndef ARCHITECTURY_IO_H
#define ARCHITECTURY_IO_H

#include "Command.h"

/**
 * @brief базовый класс для команд ввода-вывода
 */
struct IO : public Command {};

/** **************************************************** INTEGER *************************************************** **/

/**
 * @brief базовый класс для команд ввода-вывода целых чисел
 */
struct IntegerIO : public IO {};

/**
 * @brief ввод целого беззнакового числа
 * @details формат команды в файле с записью программы: t opcode s r1 15.\n
 * введенное число сохраняется в r1.\n
 * операнд - целое беззнаковое число.
 */
struct in final : public IntegerIO
{
	virtual void operator()(CPU & cpu) noexcept;
};

/**
 * @brief вывод целого беззнакового числа
 * @details формат команды в файле с записью программы: t opcode s r1 15.\n
 * выводится число из r1.\n
 * операнд - целое беззнаковое число.
 */
struct out final : public IntegerIO
{
	virtual void operator()(CPU & cpu) noexcept;
};

/**
 * @brief ввод целого знакового числа
 * @details формат команды в файле с записью программы: t opcode s r1 15.\n
 * введенное число сохраняется в r1.\n
 * операнд - целое знаковое число.
 */
struct iin final : public IntegerIO
{
	virtual void operator()(CPU & cpu) noexcept;
};

/**
 * @brief вывод целого знакового числа
 * @details формат команды в файле с записью программы: t opcode s r1 15.\n
 * выводится число из r1.\n
 * операнд - целое знаковое число.
 */
struct iout final : public IntegerIO
{
	virtual void operator()(CPU & cpu) noexcept;
};

/** ******************************************************* REAL *************************************************** **/

/**
 * @brief базовый класс для команд ввода-вывода вещественных чисел
 */
struct RealIO : public IO {};

/**
 * @brief ввод вещественного числа
 * @details формат команды в файле с записью программы: t opcode 1 r1 15.\n
 * введенное число сохраняется в r1.\n
 * операнд - вещественное число.
 */
struct rin final : public RealIO
{
	virtual void operator()(CPU & cpu) noexcept;
};

/**
 * @brief вывод вещественного числа
 * @details формат команды в файле с записью программы: t opcode 1 r1 15.\n
 * выводится число из r1.\n
 * операнд - вещественное число.
 */
struct rout final : public RealIO
{
	virtual void operator()(CPU & cpu) noexcept;
};

#endif //ARCHITECTURY_IO_H
