#ifndef ARCHITECTURY_CPU_H
#define ARCHITECTURY_CPU_H

#include <cstdint>

#include "common.h"
#include "Memory.h"
#include "commands/Command.h"
#include "commands/Arithmetic.h"
#include "commands/Move.h"
#include "commands/Jump.h"
#include "commands/Compare.h"
#include "commands/IO.h"
#include "commands/Stop.h"

#pragma pack(push, 1)
struct PSW final
{
	address_t IP; ///< CommandType Pointer, хранит адрес следующей команды
	flag_t CF : 1; ///< Carry Flag
	flag_t ZF : 1; ///< Zero Flag
	flag_t SF : 1; ///< Sign Flag
	flag_t OF : 1; ///< Overflow Flag
	word reserved : 12; ///< зарезервировано
};
#pragma pack(pop)

class CPU final
{
	PSW psw; ///< processor state word
	Command* commands[64] {nullptr}; ///< команды процессора
	CommandType current;
	
	/**
	 * @brief чтение команды из памяти в командный регистр
	 * @return возвращает количество прочитанных байт
	 */
	size_t readCommand();
	
	/// команда остановки
	friend class stop;
	
	/// арифметические операции
	friend class IntegerArithmetic;
	friend class add;
	friend class adc;
	friend class inc;
	friend class sub;
	friend class sbb;
	friend class dec;
	friend class mul;
	friend class imul;
	friend class div;
	friend class idiv;
	friend class RealArithmetic;
	friend class radd;
	friend class rsub;
	friend class rmul;
	friend class rdiv;
	
	/// операции сравнения
	friend class cmp;
	friend class rcmp;
	
	/// операции пересылки
	friend class mov;
	friend class load;
	friend class store;
	friend class fill;
	
	/// операции ввода-вывода
	friend class in;
	friend class iin;
	friend class rin;
	friend class out;
	friend class iout;
	friend class rout;
	
	/// операции перехода
	// безусловный перехода
	friend class jmp;
	friend class call;
	friend class ret;
	// условный перехода
	friend class je;
	friend class jne;
	// знаковые
	friend class jg;
	friend class jge;
	friend class jl;
	friend class jle;
	friend class jo;
	// беззнаковые
	friend class ja;
	friend class jae;
	friend class jb;
	friend class jbe;
	friend class jc;
	
public:
	word registers[8] {0}; ///< регистры общего назначения
	Memory memory; ///< память процессора
	
	CPU();
	/// установка IP
	void SetIP(const word & IP) { psw.IP = IP; }
	/// запуск процессора
	void Run();
};

#endif //ARCHITECTURY_CPU_H
