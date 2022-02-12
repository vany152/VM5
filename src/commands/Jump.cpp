#include "Jump.h"
#include "../CPU.h"

/** ************************************************** БЕЗУСЛОВНЫЕ ************************************************* **/

/**
 * @brief безусловный прямой переход
 * @details формат команды в файле с записью программы: t opcode s r1 15.\n
 * в r1 адрес перехода\n
 * s - любое
 */
void jmp::operator()(CPU & cpu) noexcept
{
	cpu.psw.IP = cpu.registers[cpu.current.command.reg_1];
}

/**
 * @brief вызов процедуры
 * @details формат команды в файле с записью программы: b opcode s r1 r2 offset.\n
 * в r1 лежит адрес процедуры\n
 * в r2 записывается адрес возврата + offset\n
 * s - любое
 */
void call::operator()(CPU & cpu) noexcept
{
	cpu.registers[cpu.current.command.reg_2] = cpu.psw.IP + cpu.current.command.offset; // запоминаем адрес возврата
	cpu.psw.IP = cpu.registers[cpu.current.command.reg_1];
}

/**
 * @brief возврат из процедуры
 * @details формат команды в файле с записью программы: t opcode s r1 15.\n
 * в r1 лежит адрес возврата\n
 * s - любое
 */
void ret::operator()(CPU & cpu) noexcept
{
	cpu.psw.IP = cpu.registers[cpu.current.command.reg_1];
}

/** *************************************************** УСЛОВНЫЕ *************************************************** **/

/**
 * @brief jump equal
 * @details формат команды в файле с записью программы: b opcode s r1 15 offset.\n
 * в r1 лежит адрес перехода\n
 * переход по адресу: r1 + offset\n
 * s - любое
 */
void je::operator()(CPU & cpu) noexcept
{
	if (cpu.psw.ZF == 1) { cpu.psw.IP = cpu.registers[cpu.current.command.reg_1] + cpu.current.command.offset; }
}

/**
 * @brief jump not equal
 * @details формат команды в файле с записью программы: b opcode s r1 15 offset.\n
 * в r1 лежит адрес перехода\n
 * переход по адресу: r1 + offset\n
 * s - любое
 */
void jne::operator()(CPU & cpu) noexcept
{
	if (cpu.psw.ZF == 0) { cpu.psw.IP = cpu.registers[cpu.current.command.reg_1] + cpu.current.command.offset; }
}

/* ********************************* Знаковые ********************************* */

/**
 * @brief jump greater
 * @details формат команды в файле с записью программы: b opcode s r1 15 offset.\n
 * в r1 лежит адрес перехода\n
 * переход по адресу: r1 + offset\n
 * s - любое
 */
void jg::operator()(CPU & cpu) noexcept
{
	if (cpu.psw.ZF == 0 && cpu.psw.SF == cpu.psw.OF)
	{ cpu.psw.IP = cpu.registers[cpu.current.command.reg_1] + cpu.current.command.offset; }
}

/**
 * @brief jump greater \ equal
 * @details формат команды в файле с записью программы: b opcode s r1 15 offset.\n
 * в r1 лежит адрес перехода\n
 * переход по адресу: r1 + offset\n
 * s - любое
 */
void jge::operator()(CPU & cpu) noexcept
{
	if (cpu.psw.SF == cpu.psw.OF)
	{ cpu.psw.IP = cpu.registers[cpu.current.command.reg_1] + cpu.current.command.offset; }
}

/**
 * @brief jump lower
 * @details формат команды в файле с записью программы: b opcode s r1 15 offset.\n
 * в r1 лежит адрес перехода\n
 * переход по адресу: r1 + offset\n
 */
 void jl::operator()(CPU & cpu) noexcept
{
	if (cpu.psw.SF != cpu.psw.OF)
	{ cpu.psw.IP = cpu.registers[cpu.current.command.reg_1] + cpu.current.command.offset; }
}

/**
 * @brief jump lower \ equal
 * @details формат команды в файле с записью программы: b opcode s r1 15 offset.\n
 * в r1 лежит адрес перехода\n
 * переход по адресу: r1 + offset\n
 * s - любое
 */
void jle::operator()(CPU & cpu) noexcept
{
	if (cpu.psw.SF != cpu.psw.OF || cpu.psw.ZF == 1)
	{ cpu.psw.IP = cpu.registers[cpu.current.command.reg_1] + cpu.current.command.offset; }
}

/**
 * @brief jump overflow
 * @details формат команды в файле с записью программы: b opcode s r1 15 offset.\n
 * в r1 лежит адрес перехода\n
 * переход по адресу: r1 + offset\n
 * s - любое
 */
 void jo::operator()(CPU & cpu) noexcept
{
	if (cpu.psw.OF == 1)
	{ cpu.psw.IP = cpu.registers[cpu.current.command.reg_1] + cpu.current.command.offset; }
}

/* ********************************* Беззнаковые ********************************* */

/**
 * @brief jump above
 * @details формат команды в файле с записью программы: b opcode s r1 15 offset.\n
 * в r1 лежит адрес перехода\n
 * переход по адресу: r1 + offset\n
 * s - любое
 */
void ja::operator()(CPU & cpu) noexcept
{
	if (cpu.psw.CF == 0 && cpu.psw.ZF == 0)
	{ cpu.psw.IP = cpu.registers[cpu.current.command.reg_1] + cpu.current.command.offset; }
}

/**
 * @brief jump above \ equal
 * @details формат команды в файле с записью программы: b opcode s r1 15 offset.\n
 * в r1 лежит адрес перехода\n
 * переход по адресу: r1 + offset\n
 * s - любое
 */
void jae::operator()(CPU & cpu) noexcept
{
	if (cpu.psw.CF == 0)
	{ cpu.psw.IP = cpu.registers[cpu.current.command.reg_1] + cpu.current.command.offset; }
}

/**
 * @brief jump below
 * @details формат команды в файле с записью программы: b opcode s r1 15 offset.\n
 * в r1 лежит адрес перехода\n
 * переход по адресу: r1 + offset\n
 * s - любое
 */
void jb::operator()(CPU & cpu) noexcept
{
	if (cpu.psw.CF == 1)
	{ cpu.psw.IP = cpu.registers[cpu.current.command.reg_1] + cpu.current.command.offset; }
}

/**
 * @brief jump below \ equal
 * @details формат команды в файле с записью программы: b opcode s r1 15 offset.\n
 * в r1 лежит адрес перехода\n
 * переход по адресу: r1 + offset\n
 * s - любое
 */
 void jbe::operator()(CPU & cpu) noexcept
{
	if (cpu.psw.CF == 1 || cpu.psw.ZF == 1)
	{ cpu.psw.IP = cpu.registers[cpu.current.command.reg_1] + cpu.current.command.offset; }
}

/**
 * @brief jump carry
 * @details формат команды в файле с записью программы: b opcode s r1 15 offset.\n
 * в r1 лежит адрес перехода\n
 * переход по адресу: r1 + offset\n
 * s - любое
 */
 void jc::operator()(CPU & cpu) noexcept
{
	if (cpu.psw.CF == 1) { cpu.psw.IP = cpu.registers[cpu.current.command.reg_1] + cpu.current.command.offset; }
}
