#ifndef OOP_PREPROCESSOR_H
#define OOP_PREPROCESSOR_H

#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdint>
#include <exception>

class Preprocessor
{
	std::stringstream buffer; ///< буфер препроцессора
	
	/// чтение содержимого файла в буфер препроцессора
	void readFile(const std::string & filename);

	/**
	 * @brief удаление комментариев из исходного кода
	 * @details коментарий начинается с символа ';'
	 */
	void removeComments() noexcept;
	
	/**
	 * @brief проверка строки на пустоту
	 * @details строка считается пустой, если она не содержит символов или содержит только
	 * пробельные символы
	 */
	static bool isRowEmpty(const std::string & row) noexcept;
	
	/// удаление пустых строк из исходного кода
	void removeEmptyRows() noexcept;
	
	/**
	 * @brief подсчет количества аргументов инструкции
	 * @details подсчитываются: opcode, s, r1, r2, offset и прочие\n
	 * не подсчитывается префикс
	 * @param command строка с инструкцией
	 * @return количество аргументов
	 */
	static int getNumberOfArguments(const std::string & command) noexcept;
	
	/// выделение префикса команды
	static char getPrefix(const std::string & command) noexcept;
	
	/**
	 * @brief проверка корректности регистров команды
	 * @details для целых коротких чисел регистры: 0 - 7, 15\n
	 * для вещественных и целых длинных: 0, 2, 4, 6, 15
	 * @param command строка с командой
	 */
	static bool isRegistersCorrect(const std::string & command) noexcept;
	
	/**
	 * @brief проверка корректности команды
	 * @details инструкция является некорректной, если:\n
	 * 1) задано неверное количество аргументов после префикса:\n
	 * 	  4 для короткой команды,\n
	 * 	  5 для длинной команды\n
	 * 	  1 для указания адреса, числа, точки старта выполнения\n
	 * 2) выбран нечетный регистр для записи вещественного или длинного целого числа
	 * @returns статус проверки: успешно / неуспешно
	 */
	bool checkCommands() noexcept;

public:
	class PreprocessorException : public std::exception
	{
		std::string _what;
	public:
		PreprocessorException(const std::string & what) : _what(what) { }
		const char * what() const noexcept override { return _what.c_str(); }
	};
	
	bool Process(const char * filename, std::stringstream & program);
};


#endif //OOP_PREPROCESSOR_H
