#ifndef ARCHITECTURY_MOVE_H
#define ARCHITECTURY_MOVE_H

#include "Command.h"

/**
 * @brief базовый класс для команд пересылки
 */
struct Move : public Command {};

/**
 * @brief пересылка регистр - регистр
 * @details формат команды в файле с записью программы: t opcode s r1 r2.\n
 * s - размер операнда (0 - 2 байта, 1 - 4 байта)\n
 * в r1 лежит номер целевого регистра\n
 * в r2 лежит номер исходного регистра
 */
struct mov final : public Move
{
	virtual void operator()(CPU & cpu) noexcept;
};

/**
 * @brief пересылка регистр - память
 * @details формат команды в файле с записью программы: b opcode s r1 r2 offset.\n
 * s - размер операнда (0 - 2 байта, 1 - 4 байта)\n
 * в r1 лежит целевой адрес памяти\n
 * в r2 лежит номер исходного регистра\n
 * offset - смещение относительно r1
 */
struct store final : public Move
{
	virtual void operator()(CPU & cpu) noexcept;
};

/**
 * @brief пересылка память - регистр
 * @details формат команды в файле с записью программы: b opcode s r1 r2 offset.\n
 * s - размер операнда (0 - 2 байта, 1 - 4 байта)\n
 * в r1 лежит номер целевого регистра\n
 * в r2 лежит исходный адрес памяти\n
 * offset - смещение относительно r2
 */
struct load final : public Move
{
	virtual void operator()(CPU & cpu) noexcept;
};

/**
 * @brief заполнение регистра числом, введенным в команде
 * @details формат команды в файле с записью программы: b opcode s r1 15 offset.\n
 * s - любое число\n
 * в r1 лежит номер целевого регистра\n
 * offset - число, которое помещается в r1
 */
struct fill final : public Move
{
	virtual void operator()(CPU & cpu) noexcept;
};

#endif //ARCHITECTURY_MOVE_H
