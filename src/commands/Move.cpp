#include <cstring>

#include "Move.h"
#include "../CPU.h"

/**
 * @brief пересылка регистр - регистр
 * @details формат команды в файле с записью программы: t opcode s r1 r2.\n
 * s - размер операнда (0 - 2 байта, 1 - 4 байта)\n
 * в r1 лежит номер целевого регистра\n
 * в r2 лежит номер исходного регистра
 */
void mov::operator()(CPU & cpu) noexcept
{
	size_t operandSize = (cpu.current.command.size) ? 4 : 2;
	memcpy(&cpu.registers[cpu.current.command.reg_1],
		   &cpu.registers[cpu.current.command.reg_2],
		   operandSize);
}

/**
 * @brief пересылка регистр - память
 * @details формат команды в файле с записью программы: b opcode s r1 r2 offset.\n
 * s - размер операнда (0 - 2 байта, 1 - 4 байта)\n
 * в r1 лежит целевой адрес памяти\n
 * в r2 лежит номер исходного регистра\n
 * offset - смещение относительно r1
 */
 void store::operator()(CPU & cpu) noexcept
{
	size_t operandSize = (cpu.current.command.size) ? 4 : 2;
	memcpy(&cpu.memory[cpu.registers[cpu.current.command.reg_1] + cpu.current.command.offset],
		   &cpu.registers[cpu.current.command.reg_2],
		   operandSize);
}

/**
 * @brief пересылка память - регистр
 * @details формат команды в файле с записью программы: b opcode s r1 r2 offset.\n
 * s - размер операнда (0 - 2 байта, 1 - 4 байта)\n
 * в r1 лежит номер целевого регистра\n
 * в r2 лежит исходный адрес памяти\n
 * offset - смещение относительно r2
 */
 void load::operator()(CPU & cpu) noexcept
{
	size_t operandSize = (cpu.current.command.size) ? 4 : 2;
	memcpy(&cpu.registers[cpu.current.command.reg_1],
		   &cpu.memory[cpu.registers[cpu.current.command.reg_2] + cpu.current.command.offset],
		   operandSize);
}

/**
 * @brief заполнение регистра числом, введенным в команде
 * @details формат команды в файле с записью программы: b opcode s r1 15 offset.\n
 * s - любое число\n
 * в r1 лежит номер целевого регистра\n
 * offset - число, которое помещается в r1
 */
 void fill::operator()(CPU & cpu) noexcept
{
	memcpy(&cpu.registers[cpu.current.command.reg_1],
		   &cpu.current.command.offset,
		   2);
}
