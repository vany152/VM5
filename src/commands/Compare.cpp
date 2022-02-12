#include <cstring>
#include <limits>
#include <cfenv>

#include "Compare.h"
#include "../CPU.h"

using std::numeric_limits;

/**
 * @brief целочисленное сравнение
 * @details вычитает второй операнд из первого и выставляет флаги SF, ZF, CF, OF.\n
 * формат команды в файле с записью программы: t opcode s r1 r2.\n
 * операнды - целые числа.\n
 * результат не сохраняется.
 */
void cmp::operator()(CPU & cpu) noexcept
{
	uint64_t op1 = 0, op2 = 0;
	size_t operandSize = (cpu.current.command.size) ? 4 : 2;
	memcpy(&op1, &cpu.registers[cpu.current.command.reg_1], operandSize);
	memcpy(&op2, &cpu.registers[cpu.current.command.reg_2], operandSize);
	
	bool op1MostBit = (op1 >> ((operandSize * 8) - 1));
	bool op2MostBit = (op2 >> ((operandSize * 8) - 1));
	op1 -= op2;
	bool resMostBit = (op1 >> ((operandSize * 8) - 1));
	
	cpu.psw.SF = resMostBit;
	cpu.psw.ZF = (op1 == 0);
	cpu.psw.CF = (op1 >> (operandSize * 8) != 0);
	// положительное - отрицательное = отрицательное
	cpu.psw.OF = (resMostBit && !op1MostBit && op2MostBit);
}

/**
 * @brief вещественное сравнение
 * @details вычитает второй операнд из первого и выставляет флаги SF, ZF, CF, OF.\n
 * формат команды в файле с записью программы: t opcode 1 r1 r2.\n
 * операнды - вещественные числа.\n
 * результат не сохраняется.
 */
void rcmp::operator()(CPU & cpu) noexcept
{
	float op1 = 0, op2 = 0;
	memcpy(&op1, &cpu.registers[cpu.current.command.reg_1], 4);
	memcpy(&op2, &cpu.registers[cpu.current.command.reg_2], 4);
	
	op1 -= op2;
	
	cpu.psw.SF = (op1 < 0);
	cpu.psw.ZF = (op1 == 0);
	cpu.psw.OF = (fetestexcept(FE_OVERFLOW) != 0);
}
