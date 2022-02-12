#ifndef ARCHITECTURY_ARITHMETIC_H
#define ARCHITECTURY_ARITHMETIC_H

#include "Command.h"

/**
 * @brief базовый класс для арифметических команд
 */
struct Arithmetic : public Command {};

/** **************************************************** INTEGER *************************************************** **/

/**
 * @brief базовый класс для арифметических операций с целыми числами
 */
struct IntegerArithmetic : public Arithmetic
{
	virtual void operator()(CPU & cpu);
	
	/**
	 * @brief вычисление результата операции
	 * @param cpu процессор (просматривается PSW)
	 * @param first первый операнд
	 * @param second второй операнд
	 */
	virtual uint64_t calculate(const CPU & cpu, const uint64_t & first, const uint64_t & second) = 0;
	
	/**
	 * @brief устанавливает флаги PSW по операндам и результату операции
	 * @param operandSize размер операнда в байтах (чтобы каждый раз его не высчитывать)
	 * @param cpu процессор, psw которого рассматривается
	 * @param op1 первый операнд
	 * @param op2 второй операнд
	 * @param result результат операции
	 */
	virtual void setFlags(
			CPU & cpu,
			const size_t & operandSize,
		    const uint64_t & op1,
		    const uint64_t & op2,
            const uint64_t & result
	) noexcept;
};

/**
 * @brief целочисленное сложение
 * @details складывает операнды и выставляет флаги SF, ZF, CF, OF.\n
 * формат команды в файле с записью программы: t opcode s r1 r2.\n
 * операнды - целые числа.
 */
struct add final : public IntegerArithmetic
{
	virtual uint64_t calculate(const CPU & cpu, const uint64_t & first, const uint64_t & second) noexcept;
};

/**
 * @brief целочисленное сложение с учетом CF
 * @details складывает два числа и CF и выставляет флаги SF, ZF, CF, OF.\n
 * формат команды в файле с записью программы: t opcode s r1 r2.\n
 * операнды - целые числа.
 */
struct adc final : public IntegerArithmetic
{
	virtual uint64_t calculate(const CPU & cpu, const uint64_t & first, const uint64_t & second) noexcept;
};

/**
 * @brief целочисленный инкремент
 * @details увеличивает операнд (целое число) на один и выставляет флаги SF, ZF, OF.\n
 * формат команды в файле с записью программы: t opcode s r1 15.\n
 * операнды - целые числа.
 */
struct inc final : public IntegerArithmetic
{
	virtual uint64_t calculate(const CPU & cpu, const uint64_t & first, const uint64_t & second) noexcept;
};

/**
 * @brief целочисленное вычитание
 * @details вычитает второй операнд из первого выставляет флаги SF, ZF, CF, OF.\n
 * формат команды в файле с записью программы: t opcode s r1 r2.\n
 * операнды - целые числа.
 */
struct sub final : public IntegerArithmetic
{
	virtual uint64_t calculate(const CPU & cpu, const uint64_t & first, const uint64_t & second) noexcept;
};

/**
 * @brief целочисленное вычитание с учетом CF
 * @details вычитает второй операнд и CF из первого выставляет флаги SF, ZF, CF, OF.\n
 * формат команды в файле с записью программы: t opcode s r1 r2.\n
 * операнды - целые числа.
 */
struct sbb final : public IntegerArithmetic
{
	virtual uint64_t calculate(const CPU & cpu, const uint64_t & first, const uint64_t & second) noexcept;
};

/**
 * @brief целочисленный декремент
 * @details уменьшает первый операнд (целое число) на один и выставляет флаги SF, ZF, OF.\n
 * формат команды в файле с записью программы: t opcode s r1 15.\n
 * операнды - целые числа.
 */
struct dec final : public IntegerArithmetic
{
	virtual uint64_t calculate(const CPU & cpu, const uint64_t & first, const uint64_t & second) noexcept;
};

/**
 * @brief целочисленное беззнаковое умножение
 * @details умножает первый операнд на второй и выставляет флаг CF.\n
 * формат команды в файле с записью программы: t opcode s r1 r2.\n
 * операнды - целые беззнаковые числа.
 */
struct mul final : public IntegerArithmetic
{
	virtual uint64_t calculate(const CPU & cpu, const uint64_t & first, const uint64_t & second) noexcept;
	virtual void setFlags(
			CPU & cpu,
			const size_t & operandSize,
			const uint64_t & op1,
			const uint64_t & op2,
            const uint64_t & result
    ) noexcept;
};

/**
 * @brief целочисленное знаковое умножение
 * @details умножает первый операнд на второй и выставляет флаг OF.\n
 * формат команды в файле с записью программы: t opcode s r1 r2.\n
 * операнды - целые знаковые числа.
 */
struct imul final : public IntegerArithmetic
{
	virtual uint64_t calculate(const CPU & cpu, const uint64_t & first, const uint64_t & second) noexcept;
	virtual void setFlags(
			CPU & cpu,
			const size_t & operandSize,
			const uint64_t & op1,
			const uint64_t & op2,
			const uint64_t & result
	) noexcept;
};

/**
 * @brief целочисленное беззнаковое деление
 * @details делит первый операнд на второй, выставляет флаг ZF\n
 * формат команды в файле с записью программы: t opcode s r1 r2.\n
 * операнды - целые знаковые числа.
 */
struct div final : public IntegerArithmetic
{
	virtual uint64_t calculate(const CPU & cpu, const uint64_t & first, const uint64_t & second);
};

/**
 * @brief целочисленное знаковое деление
 * @details делит первый операнд на второй, выставляет флаги ZF, SF\n
 * формат команды в файле с записью программы: t opcode s r1 r2.\n
 * операнды - целые знаковые числа.
 */
struct idiv final : public IntegerArithmetic
{
	virtual uint64_t calculate(const CPU & cpu, const uint64_t & first, const uint64_t & second);
};

/** ***************************************************** FLOAT **************************************************** **/

/**
 * @brief базовый класс для арифметических команд с вещественными числами
 */
struct RealArithmetic : public Arithmetic
{
	virtual void operator()(CPU & cpu) noexcept;
	
	/**
	 * @brief вычисление результата операции
	 * @param first первый операнд
	 * @param second второй операнд
	 */
	virtual float calculate(const float & first, const float & second) = 0;
	
	/**
	 * @brief устанавливает флаги PSW по результату операции
	 * @param cpu процессор, psw которого рассматривается
	 * @param result результат операции
	 */
	virtual void setFlags(CPU & cpu, const float & result) noexcept;
};

/**
 * @brief вещественное сложение
 * @details складывает операнды и выставляет флаги SF, ZF, OF\n
 * формат команды в файле с записью программы: t opcode 1 r1 r2.\n
 * операнды - вещественные числа.
 */
struct radd final : public RealArithmetic
{
	virtual float calculate(const float & first, const float & second) noexcept;
};

/**
 * @brief вещественное вычитание
 * @details вычитает второй операнд из первого и выставляет флаги SF, ZF, OF\n
 * формат команды в файле с записью программы: t opcode 1 r1 r2.\n
 * операнды - вещественные числа.
 */
struct rsub final : public RealArithmetic
{
	virtual float calculate(const float & first, const float & second) noexcept;
};

/**
 * @brief вещественное умножение
 * @details умножает операнды и выставляет флаги SF, ZF, OF\n
 * формат команды в файле с записью программы: t opcode 1 r1 r2.\n
 * операнды - вещественные числа.
 */
struct rmul final : public RealArithmetic
{
	virtual float calculate(const float & first, const float & second) noexcept;
};

/**
 * @brief вещественное деление
 * @details делит первый операнд на второй и выставляет флаги SF, OF\n
 * формат команды в файле с записью программы: t opcode 1 r1 r2.\n
 * операнды - вещественные числа.
 */
struct rdiv final : public RealArithmetic
{
	virtual float calculate(const float & first, const float & second) noexcept;
};

#endif //ARCHITECTURY_ARITHMETIC_H
