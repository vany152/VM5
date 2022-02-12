#include <cstring>

#include "CPU.h"

CPU::CPU()
{
	/*
	 * добавление команд
	 */
	/// остановки - 0
	commands[0] = new stop;

	/// арифметические 1..19
	// сложение
	commands[1] = new add();
	commands[2] = new adc();
	commands[3] = new inc();
	commands[4] = new radd();
	// вычитание
	commands[5] = new sub();
	commands[6] = new sbb();
	commands[7] = new dec();
	commands[8] = new rsub();
	// умножение
	commands[10] = new mul();
	commands[11] = new imul();
	commands[12] = new rmul();
	// деление
	commands[15] = new div();
	commands[16] = new idiv();
	commands[17] = new rdiv();
	
	/// сравнение 20..24
	commands[20] = new cmp();
	commands[21] = new rcmp();
	
	/// пересылки 25..29
	commands[25] = new mov();
	commands[26] = new load();
	commands[27] = new store();
	commands[28] = new fill();
	
	/// ввод / вывод 30..39
	// ввод
	commands[30] = new in();
	commands[31] = new iin();
	commands[32] = new rin();
	// вывод
	commands[35] = new out();
	commands[36] = new iout();
	commands[37] = new rout();
	
	/// переходы 40..70
	// безусловные
	commands[40] = new jmp();
	commands[41] = new call();
	commands[42] = new ret();
	// условные
	commands[43] = new je();
	commands[44] = new jne();
	// знаковые
	commands[45] = new jg();
	commands[46] = new jge();
	commands[47] = new jl();
	commands[48] = new jle();
	commands[49] = new jo();
	// беззнаковые
	commands[50] = new ja;
	commands[51] = new jae;
	commands[52] = new jb;
	commands[53] = new jbe;
	commands[54] = new jc;
}

/**
 * @brief чтение команды из памяти в командный регистр
 * @return возвращает количество прочитанных байт
 */
size_t CPU::readCommand()
{
	size_t bytesRead = 0;
	
	current.raw[0] = memory[psw.IP + bytesRead++];
	current.raw[1] = memory[psw.IP + bytesRead++];
	
	if (current.command.length == 1)
	{
		current.raw[2] = memory[psw.IP + bytesRead++];
		current.raw[3] = memory[psw.IP + bytesRead++];
	}
	
	return bytesRead;
}

/// запуск процессора
void CPU::Run()
{
	do
	{
		size_t length = readCommand();
		psw.IP += length;
		
		commands[current.command.opcode]->operator()(*this);
	} while (current.command.opcode != 0); // пока не попалась команда stop (КОП = 0)
}
