#ifndef ARCHITECTURY_COMPARE_H
#define ARCHITECTURY_COMPARE_H

#include "Command.h"
#include "Arithmetic.h"

/**
 * @brief базовый класс для команд сравнения
 */
struct Compare : public Command {};

/**
 * @brief целочисленное сравнение
 * @details вычитает второй операнд из первого и выставляет флаги SF, ZF, CF, OF.\n
 * формат команды в файле с записью программы: t opcode s r1 r2.\n
 * операнды - целые числа.\n
 * результат не сохраняется.
 */
struct cmp final : public Compare
{
	virtual void operator()(CPU & cpu) noexcept;
};

/**
 * @brief вещественное сравнение
 * @details вычитает второй операнд из первого и выставляет флаги SF, ZF, CF, OF.\n
 * формат команды в файле с записью программы: t opcode 1 r1 r2.\n
 * операнды - вещественные числа.\n
 * результат не сохраняется.
 */
struct rcmp final : public Compare
{
	virtual void operator()(CPU & cpu) noexcept;
};

#endif //ARCHITECTURY_COMPARE_H
