#include <cstring>
#include <iostream>

#include "common.h"
#include "CPU.h"
#include "Preprocessor.h"

using namespace std;

#ifdef DEBUG
#include <filesystem>
namespace fs = filesystem;
#endif // DEBUG

/// обработка исключений
void handleError(const std::string & what)
{
	std::cerr << what << std::endl;
	system("pause");
	exit(EXIT_FAILURE);
}

/// функция загрузчик
void loader(CPU & cpu, stringstream & program)
{
	address_t currentMemoryAddress = 0;
	char prefix;
	int opcode;
	int s;
	int reg_1;
	int reg_2;
	int offset;
	
	string tempRow;
	while (!program.eof())
	{
		getline(program, tempRow, '\n');
		istringstream row(tempRow);
		row >> prefix;
		
		switch (prefix)
		{
			case 'b': // big command
			{
				row >> opcode >> s >> reg_1 >> reg_2 >> offset;

				CommandType command;
				command.command.length = 1; // 1 бит
				command.command.opcode = opcode; // 6 бит
				command.command.size = s; // 1 бит
				command.command.reg_1 = reg_1; // 4 бита
				command.command.reg_2 = reg_2; // 4 бита
				command.command.offset = offset; // 16 бит

				for (size_t i = 0; i < 4; ++i) { cpu.memory[currentMemoryAddress + i] = command.raw[i]; }
				
				currentMemoryAddress += 4;
				break;
			}
			case 't': // tiny command
			{
				row >> opcode >> s >> reg_1 >> reg_2;
				
				CommandType command;
				command.command.length = 0; // 1 бит
				command.command.opcode = opcode; // 6 бит
				command.command.size = s; // 1 бит
				command.command.reg_1 = reg_1; // 4 бита
				command.command.reg_2 = reg_2; // 4 бита
				
				cpu.memory[currentMemoryAddress] = command.raw[0];
				cpu.memory[currentMemoryAddress + 1] = command.raw[1];
				
				currentMemoryAddress += 2;
				break;
			}
			case 's': // short number
			{
				uint16_t number;
				row >> number;
				memcpy(&cpu.memory[currentMemoryAddress], &number, 2);
				
				currentMemoryAddress += 2;
				break;
			}
			case 'l': // long number
			{
				uint32_t number;
				row >> number;
				memcpy(&cpu.memory[currentMemoryAddress], &number, 4);
				
				currentMemoryAddress += 4;
				break;
			}
			case 'r': // real number
			{
				float number;
				row >> number;
				memcpy(&cpu.memory[currentMemoryAddress], &number, 4);
				
				currentMemoryAddress += 4;
				break;
			}
			case 'a': // address
			{
				row >> currentMemoryAddress;
				break;
			}
			case 'e': // execute
			{
				address_t start;
				row >> start;
				cpu.SetIP(start);
				break;
			}
			default:
			{
				throw "не удалось определить команду";
			}
		}
	}
	
	program.clear();
	program.seekg(ios::beg);
}

int main(int argc, char* argv[])
{
	system("chcp 65001"); // utf-8 кодировка консоли

#ifdef DEBUG
	cout << "argc = " << argc << "; argv: " << endl;
	for (int i = 0; i < argc; ++i) { cout << argv[i] << endl; }
	cout << endl;
	
	fs::path basePath = "D:/study/architectury/architectury/coursework/src";
//	fs::path filename = "tests/arithmetic tests/int add, adc, inc.txt";
//	fs::path filename = "tests/arithmetic tests/int sub, sbb, dec.txt";
//	fs::path filename = "tests/arithmetic tests/short mul, imul, div, idiv.txt";
//	fs::path filename = "tests/arithmetic tests/long mul, imul, div, idiv.txt";
//	fs::path filename = "tests/arithmetic tests/real radd, rsub, rmul, rdiv.txt";
//	fs::path filename = "tests/io, move.txt";
//	fs::path filename = "tests/jump test/call, ret.txt";
//	fs::path filename = "tests/jump test/je, jne.txt";
//	fs::path filename = "tests/jump test/jg, jge, jl, jle, jo.txt";
//	fs::path filename = "tests/jump test/ja, jae, jb, jbe, jc.txt";
	fs::path filename = "demo/cycle.txt";
#endif // DEBUG
	
	Preprocessor preprocessor;
	stringstream program;
	try
	{
#ifdef DEBUG
		string fullPath = (basePath / filename).string();
		bool success = preprocessor.Process(fullPath.c_str(), program);
#else
		bool success = preprocessor.Process(argv[1], program);
#endif // DEBUG
		
		if (!success)
		{
			system("pause");
			exit(EXIT_FAILURE);
		}
	}
	catch (exception & err)
	{
		handleError(err.what());
	}
	
	CPU cpu;
	try { loader(cpu, program); }
	catch (char * what) { handleError(string(what)); }
	try { cpu.Run(); }
	catch (exception & err) { handleError(err.what()); }
	
	cout << endl;
	system("pause");
	return 0;
}