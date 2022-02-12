#include <string>
#include <iostream>

#include "IO.h"
#include "../CPU.h"

using namespace std;

/** **************************************************** INTEGER *************************************************** **/

/**
 * @brief ввод целого беззнакового числа
 * @details формат команды в файле с записью программы: t opcode s r1 15.\n
 * введенное число сохраняется в r1.\n
 * операнд - целое беззнаковое число.
 */
void in::operator()(CPU & cpu) noexcept
{
	uint64_t number;
	size_t operandSize = (cpu.current.command.size) ? 4 : 2;
	/*
	 * при вводе с помощью istream >> в переменную числа, размер которого превышает
	 * размер переменной не происходит закономерное переполнение, и в переменную
	 * из числа попадает ее максимальное значение, остальные биты переменной остаются
	 * в буфере и портят следующие операции ввода.
	 * для попадания в переменную number корректного значения при переполнении,
	 * число попадает в переменную большего размера, после чего лишние биты отсекаются.
	 */
	cout << "in << "; cin >> number;
	if (cpu.current.command.size == 1) { number = static_cast<uint32_t>(number); }
	else { number = number = static_cast<uint16_t>(number); }
	memcpy(&cpu.registers[cpu.current.command.reg_1], &number, operandSize);
}

/**
 * @brief вывод целого беззнакового числа
 * @details формат команды в файле с записью программы: t opcode s r1 15.\n
 * выводится число из r1.\n
 * операнд - целое беззнаковое число.
 */
void out::operator()(CPU & cpu) noexcept
{
	uint32_t number = 0;
	size_t operandSize = (cpu.current.command.size) ? 4 : 2;
	memcpy(&number, &cpu.registers[cpu.current.command.reg_1], operandSize);
	/*
	 * так как команда работает и с длинными, и с короткими числами, операнд сохраняется в переменной
	 * большего размера - 4 байта, из-за чего команда не может отловить переполнение числа и вывести
	 * число, большее 65535.
	 * для избежания подобных ситуаций короткое число явно преобразуется к короткому
	 */
	if (operandSize == 2) { number = static_cast<uint16_t>(number); }
	cout << "out >> " << number << endl;
}

/**
 * @brief ввод целого знакового числа
 * @details формат команды в файле с записью программы: t opcode s r1 15.\n
 * введенное число сохраняется в r1.\n
 * операнд - целое знаковое число.
 */
void iin::operator()(CPU & cpu) noexcept
{
	int64_t number;
	size_t operandSize = (cpu.current.command.size) ? 4 : 2;
	/*
	 * при вводе с помощью istream >> в переменную числа, размер которого превышает
	 * размер переменной не происходит закономерное переполнение, и в переменную
	 * из числа попадает ее максимальное значение, остальные биты переменной остаются
	 * в буфере и портят следующие операции ввода.
	 * для попадания в переменную number корректного значения при переполнении,
	 * число попадает в переменную большего размера, после чего лишние биты отсекаются.
	 */
	cout << "iin << "; cin >> number;
	if (operandSize == 4) { number = static_cast<int32_t>(number); }
	else { number = static_cast<int32_t>(number); }
	memcpy(&cpu.registers[cpu.current.command.reg_1], &number, operandSize);
}

/**
 * @brief вывод целого знакового числа
 * @details формат команды в файле с записью программы: t opcode s r1 15.\n
 * выводится число из r1.\n
 * операнд - целое знаковое число.
 */
void iout::operator()(CPU & cpu) noexcept
{
	int32_t number = 0;
	size_t operandSize = (cpu.current.command.size) ? 4 : 2;
	memcpy(&number, &cpu.registers[cpu.current.command.reg_1], operandSize);
	/*
	 * так как команда работает и с длинными, и с короткими числами, операнд сохраняется в переменной
	 * большего размера - 4 байта, из-за чего команда не может отловить переполнение короткого числа
	 * и вместо отрицательного значения вывести число, большее 32767.
	 * для избежания подобных ситуаций короткое число явно преобразуется к короткому
	 */
	if (operandSize == 2) { number = static_cast<int16_t>(number); }
	else { number = static_cast<int32_t>(number); }
	cout << "iout >> " << number << endl;
}

/** ******************************************************* REAL *************************************************** **/

/**
 * @brief ввод вещественного числа
 * @details формат команды в файле с записью программы: t opcode 1 r1 15.\n
 * введенное число сохраняется в r1.\n
 * операнд - вещественное число.
 */
void rin::operator()(CPU & cpu) noexcept
{
	float number;
	cout << "rin << "; cin >> number;
	memcpy(&cpu.registers[cpu.current.command.reg_1], &number, 4);
}

/**
 * @brief вывод вещественного числа
 * @details формат команды в файле с записью программы: t opcode 1 r1 15.\n
 * выводится число из r1.\n
 * операнд - вещественное число.
 */
void rout::operator()(CPU & cpu) noexcept
{
	float number = 0;
	memcpy(&number, &cpu.registers[cpu.current.command.reg_1], 4);
	cout << "rout >> "<<  number << endl;
}
