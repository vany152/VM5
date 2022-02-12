#include <cstring>
#include <cstdint>
#include <cfenv>
#include <limits>

#include "Arithmetic.h"
#include "../CPU.h"

using std::numeric_limits;

void IntegerArithmetic::operator()(CPU & cpu)
{
	uint64_t op1 = 0, op2 = 0;
	size_t operandSize = (cpu.current.command.size) ? 4 : 2; // размер операнда в байтах
	memcpy(&op1, &cpu.registers[cpu.current.command.reg_1], operandSize);
	memcpy(&op2, &cpu.registers[cpu.current.command.reg_2], operandSize);
	
	uint64_t result = calculate(cpu, op1, op2);
	setFlags(cpu, operandSize, op1, op1, result);

	memcpy(&cpu.registers[cpu.current.command.reg_1], &result, operandSize);
}

/**
 * @brief устанавливает флаги PSW по операндам и результату операции
 * @param operandSize размер операнда в байтах (чтобы каждый раз его не высчитывать)
 * @param cpu процессор, psw которого рассматривается
 * @param op1 первый операнд
 * @param op2 второй операнд
 * @param result результат операции
 */
void IntegerArithmetic::setFlags(
		CPU & cpu,
		const size_t & operandSize,
		const uint64_t & op1,
		const uint64_t & op2,
		const uint64_t & result
) noexcept
{
	bool op1MostBit = (op1 >> (operandSize * 8 - 1));
	bool op2MostBit = (op2 >> (operandSize * 8 - 1));
	bool resMostBit = (result >> (operandSize * 8 - 1));
	
	cpu.psw.SF = resMostBit;
	cpu.psw.ZF = (result == 0);
	cpu.psw.CF = (result >> (operandSize * 8) != 0);
	/*
     * положительное + положительное = отрицательное
     */
	cpu.psw.OF = (resMostBit && !op1MostBit && !op2MostBit);
}

/**
 * @brief целочисленное сложение
 * @details складывает операнды и выставляет флаги SF, ZF, CF, OF.\n
 * формат команды в файле с записью программы: t opcode s r1 r2.\n
 * операнды - целые числа.
 */
uint64_t add::calculate(const CPU & cpu, const uint64_t & first, const uint64_t & second) noexcept
{
	return first + second;
}

/**
 * @brief целочисленное сложение с учетом CF
 * @details складывает два числа и CF и выставляет флаги SF, ZF, CF, OF.\n
 * формат команды в файле с записью программы: t opcode s r1 r2.\n
 * операнды - целые числа.
 */
uint64_t adc::calculate(const CPU & cpu, const uint64_t & first, const uint64_t & second) noexcept
{
	return first + second + cpu.psw.CF;
}

/**
 * @brief целочисленный инкремент
 * @details увеличивает первый операнд (целое число) на один и выставляет флаги SF, ZF, OF.\n
 * операнды - целые числа.\n
 * формат команды в файле с записью программы: t opcode s r1 15.
 */
uint64_t inc::calculate(const CPU & cpu, const uint64_t & first, const uint64_t & second) noexcept
{
	return first + 1;
}

/**
 * @brief целочисленное вычитание
 * @details вычитает второй операнд из первого выставляет флаги SF, ZF, CF, OF.\n
 * формат команды в файле с записью программы: t opcode s r1 r2.\n
 * операнды - целые числа.
 */
uint64_t sub::calculate(const CPU & cpu, const uint64_t & first, const uint64_t & second) noexcept
{
	return first - second;
}

/**
 * @brief целочисленное вычитание с учетом CF
 * @details вычитает второй операнд и CF из первого выставляет флаги SF, ZF, CF, OF.\n
 * формат команды в файле с записью программы: t opcode s r1 r2.\n
 * операнды - целые числа.
 */
uint64_t sbb::calculate(const CPU & cpu, const uint64_t & first, const uint64_t & second) noexcept
{
	return first - second - cpu.psw.CF;
}

/**
 * @brief целочисленный декремент
 * @details уменьшает первый операнд (целое число) на один и выставляет флаги SF, ZF, OF.\n
 * формат команды в файле с записью программы: t opcode s r1 15.\n
 * операнды - целые числа.
 */
uint64_t dec::calculate(const CPU & cpu, const uint64_t & first, const uint64_t & second) noexcept
{
	return first - 1;
}

/**
 * @brief целочисленное беззнаковое умножение
 * @details умножает первый операнд на второй и выставляет флаг CF.\n
 * формат команды в файле с записью программы: t opcode s r1 r2.\n
 * операнды - целые беззнаковые числа.
 */
uint64_t mul::calculate(const CPU & cpu, const uint64_t & first, const uint64_t & second) noexcept
{
	return first * second;
}

void mul::setFlags(
		CPU & cpu,
		const size_t & operandSize,
		const uint64_t & op1,
		const uint64_t & op2,
		const uint64_t & result
) noexcept
{
	/*
	 * OF и CF выставляются следующим образом:
	 * если старшие cpu.current.command.size / 2 битов числа равны нулю,
	 * оба флага сброшены, иначе оба флага выставляются
	 */
	cpu.psw.OF = cpu.psw.CF = (result >> (operandSize * 8) != 0);
}

/**
 * @brief целочисленное знаковое умножение
 * @details умножает первый операнд на второй и выставляет флаг OF.\n
 * формат команды в файле с записью программы: t opcode s r1 r2.\n
 * операнды - целые знаковые числа.
 */
uint64_t imul::calculate(const CPU & cpu, const uint64_t & first, const uint64_t & second) noexcept
{
	int64_t op1 = first, op2 = second;
	
	/*
	 * в случае, если в памяти будет храниться короткое отрицательное число, например, -1,
	 * байты числа будут скопированы в переменную большего размера, что не вызовет
	 * преобразование 65535 в -1, и в op1 или op2 будет сохранено неправильное значение,
	 * что приведет к неправильному результату умножения.
	 * для избежания подобных ситуаций число преобразуется к знаковому.
	 */
	if (cpu.current.command.size == 0)
	{
		op1 = static_cast<int16_t>(op1);
		op2 = static_cast<int16_t>(op2);
	}
	else
	{
		op1 = static_cast<int32_t>(op1);
		op2 = static_cast<int32_t>(op2);
	}
	
	return op1 * op2;
}

void imul::setFlags(
		CPU & cpu,
		const size_t & operandSize,
		const uint64_t & op1,
		const uint64_t & op2,
		const uint64_t & result
) noexcept
{
	/*
	 * OF и CF выставляются следующим образом:
	 * если старшие cpu.current.command.size / 2 + 1 битов числа равны нулю,
	 * оба флага сброшены, иначе оба флага выставляются
	 */
	cpu.psw.OF = cpu.psw.CF = (result >> (operandSize * 8 - 1) != 0);
}

/**
 * @brief целочисленное беззнаковое деление
 * @details делит первый операнд на второй, выставляет флаг ZF\n
 * формат команды в файле с записью программы: t opcode s r1 r2.\n
 * операнды - целые знаковые числа.
 */
uint64_t div::calculate(const CPU & cpu, const uint64_t & first, const uint64_t & second)
{
	return first / second;
}

/**
 * @brief целочисленное знаковое деление
 * @details делит первый операнд на второй, выставляет флаги ZF, SF\n
 * формат команды в файле с записью программы: t opcode s r1 r2.\n
 * операнды - целые знаковые числа.
 */
uint64_t idiv::calculate(const CPU & cpu, const uint64_t & first, const uint64_t & second)
{
	int64_t op1 = first, op2 = second;
	
	/*
	 * в случае, если в памяти будет храниться короткое отрицательное число, например, -1,
	 * байты числа будут скопированы в переменную большего размера, что не вызовет
	 * преобразование 65535 в -1, и в op1 или op2 будет сохранено неправильное значение,
	 * что приведет к неправильному результату деления.
	 * для избежания подобных ситуаций число преобразуется к знаковому.
	 */
	if (cpu.current.command.size == 0)
	{
		op1 = static_cast<int16_t>(op1);
		op2 = static_cast<int16_t>(op2);
	}
	else
	{
		op1 = static_cast<int32_t>(op1);
		op2 = static_cast<int32_t>(op2);
	}
	
	return op1 / op2;
}

/** ***************************************************** FLOAT **************************************************** **/

void RealArithmetic::operator()(CPU & cpu) noexcept
{
	float op1 = 0, op2 = 0;
	memcpy(&op1, &cpu.registers[cpu.current.command.reg_1], 4);
	memcpy(&op2, &cpu.registers[cpu.current.command.reg_2], 4);
	
	float result = calculate(op1, op2);
	setFlags(cpu, result);
	
	memcpy(&cpu.registers[cpu.current.command.reg_1], &result, 4);
}

/**
 * @brief устанавливает флаги PSW по результату операции
 * @param cpu процессор, psw которого рассматривается
 * @param result результат операции
 */
void RealArithmetic::setFlags(CPU & cpu, const float & result) noexcept
{
	cpu.psw.SF = (result < 0);
	cpu.psw.ZF = (result == 0);
	cpu.psw.OF = (fetestexcept(FE_OVERFLOW) != 0);
}

/**
 * @brief вещественное сложение
 * @details складывает операнды и выставляет флаги SF, ZF, OF\n
 * формат команды в файле с записью программы: t opcode 1 r1 r2.\n
 * операнды - вещественные числа.
 */
float radd::calculate(const float & first, const float & second) noexcept
{
	return first + second;
}

/**
 * @brief вещественное вычитание
 * @details вычитает второй операнд из первого и выставляет флаги SF, ZF, OF\n
 * формат команды в файле с записью программы: t opcode 1 r1 r2.\n
 * операнды - вещественные числа.
 */
float rsub::calculate(const float & first, const float & second) noexcept
{
	return first - second;
}

/**
 * @brief вещественное умножение
 * @details умножает операнды и выставляет флаги SF, ZF, OF\n
 * формат команды в файле с записью программы: t opcode 1 r1 r2.\n
 * операнды - вещественные числа.
 */
float rmul::calculate(const float & first, const float & second) noexcept
{
	return first * second;
}

/**
 * @brief вещественное деление
 * @details делит первый операнд на второй и выставляет флаги SF, OF\n
 * формат команды в файле с записью программы: t opcode 1 r1 r2.\n
 * операнды - вещественные числа.
 */
float rdiv::calculate(const float & first, const float & second) noexcept
{
	return first / second;
}
