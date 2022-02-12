#ifndef ARCHITECTURY_JUMP_H
#define ARCHITECTURY_JUMP_H

#include "Command.h"

/**
 * @brief базовый класс для команд перехода
 */
struct Jump : public Command {};

/** ************************************************** БЕЗУСЛОВНЫЕ ************************************************* **/

/**
 * @brief базовый класс для команд безусловного перехода
 */
struct UnconditionalJump : public Jump {};

/**
 * @brief безусловный прямой переход
 * @details формат команды в файле с записью программы: t opcode s r1 15.\n
 * в r1 адрес перехода\n
 * s - любое
 */
struct jmp final : public UnconditionalJump
{
	virtual void operator()(CPU & cpu) noexcept;
};

/**
 * @brief вызов процедуры
 * @details формат команды в файле с записью программы: b opcode s r1 r2 offset.\n
 * в r1 лежит адрес процедуры\n
 * в r2 записывается адрес возврата + offset\n
 * s - любое
 */
struct call final : public UnconditionalJump
{
	virtual void operator()(CPU & cpu) noexcept;
};

/**
 * @brief возврат из процедуры
 * @details формат команды в файле с записью программы: t opcode s r1 15.\n
 * в r1 лежит адрес возврата\n
 * s - любое
 */
struct ret final : public UnconditionalJump
{
	virtual void operator()(CPU & cpu) noexcept;
};

/** *************************************************** УСЛОВНЫЕ *************************************************** **/

/**
 * @brief базовый класс для команд условного перехода
 */
struct ConditionalJump : public Jump {};

/**
 * @brief jump equal
 * @details формат команды в файле с записью программы: b opcode s r1 15 offset.\n
 * в r1 лежит адрес перехода\n
 * переход по адресу: r1 + offset\n
 * s - любое
 */
struct je final : public ConditionalJump
{
	virtual void operator()(CPU & cpu) noexcept;
};

/**
 * @brief jump not equal
 * @details формат команды в файле с записью программы: b opcode s r1 15 offset.\n
 * в r1 лежит адрес перехода\n
 * переход по адресу: r1 + offset\n
 * s - любое
 */
struct jne final : public ConditionalJump
{
	virtual void operator()(CPU & cpu) noexcept;
};

/* ********************************* Знаковые ********************************* */

/**
 * @brief jump greater
 * @details формат команды в файле с записью программы: b opcode s r1 15 offset\n.
 * в r1 лежит адрес перехода\n
 * переход по адресу: r1 + offset\n
 * s - любое
 */
struct jg final : public ConditionalJump
{
	virtual void operator()(CPU & cpu) noexcept;
};

/**
 * @brief jump greater \ equal
 * @details формат команды в файле с записью программы: b opcode s r1 15 offset.\n
 * в r1 лежит адрес перехода\n
 * переход по адресу: r1 + offset\n
 * s - любое
 */
struct jge final : public ConditionalJump
{
	virtual void operator()(CPU & cpu) noexcept;
};

/**
 * @brief jump lower
 * @details формат команды в файле с записью программы: b opcode s r1 15 offset.\n
 * в r1 лежит адрес перехода\n
 * переход по адресу: r1 + offset\n
 * s - любое
 */
 struct jl final : public ConditionalJump
{
	virtual void operator()(CPU & cpu) noexcept;
};

/**
 * @brief jump lower \ equal
 * @details формат команды в файле с записью программы: b opcode s r1 15 offset.\n
 * в r1 лежит адрес перехода\n
 * переход по адресу: r1 + offset\n
 * s - любое
 */
struct jle final : public ConditionalJump
{
	virtual void operator()(CPU & cpu) noexcept;
};

/**
 * @brief jump overflow
 * @details формат команды в файле с записью программы: b opcode s r1 15 offset.\n
 * в r1 лежит адрес перехода\n
 * переход по адресу: r1 + offset\n
 * s - любое
 */
struct jo final : public ConditionalJump
{
	virtual void operator()(CPU & cpu) noexcept;
};

/* ********************************* Беззнаковые ********************************* */

/**
 * @brief jump above
 * @details формат команды в файле с записью программы: b opcode s r1 15 offset.\n
 * в r1 лежит адрес перехода\n
 * переход по адресу: r1 + offset\n
 * s - любое
 */
struct ja final : public ConditionalJump
{
	virtual void operator()(CPU & cpu) noexcept;
};

/**
 * @brief jump above \ equal
 * @details формат команды в файле с записью программы: b opcode s r1 15 offset.\n
 * в r1 лежит адрес перехода\n
 * переход по адресу: r1 + offset\n
 * s - любое
 */struct jae final : public ConditionalJump
{
	virtual void operator()(CPU & cpu) noexcept;
};

/**
 * @brief jump below
 * @details формат команды в файле с записью программы: b opcode s r1 15 offset.\n
 * в r1 лежит адрес перехода\n
 * переход по адресу: r1 + offset\n
 * s - любое
 */
struct jb final : public ConditionalJump
{
	virtual void operator()(CPU & cpu) noexcept;
};

/**
 * @brief jump below \ equal
 * @details формат команды в файле с записью программы: b opcode s r1 15 offset.\n
 * в r1 лежит адрес перехода\n
 * переход по адресу: r1 + offset\n
 * s - любое
 */
struct jbe final : public ConditionalJump
{
	virtual void operator()(CPU & cpu) noexcept;
};

/**
 * @brief jump carry
 * @details формат команды в файле с записью программы: b opcode s r1 15 offset.\n
 * в r1 лежит адрес перехода\n
 * переход по адресу: r1 + offset\n
 * s - любое
 */
struct jc final : public ConditionalJump
{
	virtual void operator()(CPU & cpu) noexcept;
};

#endif //ARCHITECTURY_JUMP_H
