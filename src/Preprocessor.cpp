#include <cctype>

#include "Preprocessor.h"

using namespace std;

/// чтение содержимого файла в буфер препроцессора
void Preprocessor::readFile(const std::string & filename)
{
	ifstream fin(filename);
	if (!fin.is_open()) { throw PreprocessorException("не удалось открыть файл " + filename); }
	
	for (string row; getline(fin, row, '\n'); buffer << row << '\n') {}
	
	buffer.seekp(ios::beg);
	
	fin.close();
}

/**
 * @brief удаление комментариев из исходного кода
 * @details коментарий начинается с символа ';'
 */
void Preprocessor::removeComments() noexcept
{
	string row;
	stringstream newInternalBuffer;
	
	while (!buffer.eof())
	{
		getline(buffer, row, '\n');
		
		size_t commentStarts = row.find(';');
		if (commentStarts != string::npos) { row.erase(commentStarts); }
		
		newInternalBuffer << row << '\n';
	}
	
	newInternalBuffer.clear();
	newInternalBuffer.seekp(ios::beg);
	buffer.swap(newInternalBuffer);
}

/**
 * @brief проверка строки на пустоту
 * @details строка считается пустой, если она не содержит символов или содержит только
 * пробельные символы
 */
bool Preprocessor::isRowEmpty(const string & row) noexcept
{
	if (row.empty()) { return true; } // для удаления строк, не содержащих ни одного символа
	
	bool empty = true;
	for (auto symbol = row.front(); symbol != row.back() && empty; ++symbol)
	{
		if (!isspace(symbol))
		{
			empty = false;
		}
	}
	
	return empty;
}

/// удаление пустых строк из исходного кода
void Preprocessor::removeEmptyRows() noexcept
{
	stringstream newInternalBuffer;
	
	for (string row; getline(buffer, row, '\n');)
	{
		if (!isRowEmpty(row))
		{
			newInternalBuffer << row << '\n';
		}
	}
	
	newInternalBuffer.clear();
	newInternalBuffer.seekg(ios::beg);
	buffer.swap(newInternalBuffer);
}

/**
 * @brief подсчет количества аргументов инструкции
 * @details подсчитываются: opcode, s, r1, r2, offset и прочие\n
 * не подсчитывается префикс
 * @param command строка с инструкцией
 * @return количество аргументов
 */
int Preprocessor::getNumberOfArguments(const string & command) noexcept
{
	istringstream iss(command);
	int numberOfArguments = 0;
	
	for (string str; iss >> str; ++numberOfArguments) {}
	
	return --numberOfArguments;
}

/// выделение префикса команды
char Preprocessor::getPrefix(const string & command) noexcept
{
	istringstream iss(command);
	char prefix;
	iss >> prefix;
	
	return prefix;
}

/**
 * @brief проверка корректности регистров команды
 * @details для целых коротких чисел регистры: 0 - 7, 15\n
 * для вещественных и целых длинных: 0, 2, 4, 6, 15
 * @param command строка с командой
 */
bool Preprocessor::isRegistersCorrect(const string & command) noexcept
{
	char prefix;
	int opcode;
	int s;
	int reg_1;
	int reg_2;
	
	istringstream iss(command);
	iss >> prefix >> opcode >> s >> reg_1 >> reg_2;
	
	bool correct;
	if (s == 0)
	{
		correct = (
				(0 <= reg_1 && reg_1 <= 7 || reg_1 == 15) &&
				(0 <= reg_2 && reg_2 <= 7 || reg_2 == 15)
		);
	}
	else
	{
		correct = (
				(0 <= reg_1 && reg_1 <= 7 && reg_1 % 2 == 0 || reg_1 == 15) &&
				(0 <= reg_2 && reg_2 <= 7 && reg_2 % 2 == 0 || reg_2 == 15)
		);
	}
	
	return correct;
}

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
bool Preprocessor::checkCommands() noexcept
{
	bool success = true;
	
	size_t rowNumber = 1;
	for (string command; getline(buffer, command, '\n'); ++rowNumber)
	{
		if (!command.empty())
		{
			char prefix = getPrefix(command);
			int numberOfArguments = getNumberOfArguments(command);
			
			if ((prefix == 't' && numberOfArguments != 4) ||
			    (prefix == 'b' && numberOfArguments != 5) ||
			    (((prefix == 'a') ||
			      (prefix == 's') ||
			      (prefix == 'l') ||
			      (prefix == 'r') ||
			      (prefix == 'e')) && numberOfArguments != 1))
			{
				cerr << "неверная число аргументов инструкции в строке " << rowNumber << endl;
				success = false;
			}
			else if (!isRegistersCorrect(command) && (prefix == 't' || prefix == 'b'))
			{
				cerr << "неверные регистры команды в строке " << rowNumber << endl;
				success = false;
			}
		}
	}
	
	buffer.clear();
	buffer.seekg(ios::beg);
	
	return success;
}

bool Preprocessor::Process(const char * filename, stringstream & program)
{
	readFile(filename);
	removeComments();
	bool success = checkCommands();
	if (success) // если в исходном коде программы ВМ не было ошибок
	{
		removeEmptyRows();
		program.swap(buffer);
	}

	return success;
}
