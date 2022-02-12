#ifndef OOP_COMMON_H
#define OOP_COMMON_H

#include <cstdint>

//#define DEBUG // todo закомментить в релизе!

using address_t = uint16_t;
using flag_t = uint8_t;
using byte_t = uint8_t;
using word = uint16_t;
using dword = uint32_t;

#pragma pack(push, 1)
union CommandType
{
	struct
	{
		byte_t length : 1; ///< бит размера команды в байтах (0 - короткая команда, 1 - длинная)
		byte_t opcode : 6; ///< код операции, 6 бит
		byte_t size : 1; ///< бит размера операнда в байтах (0 - 2 байта, 1 - 4 байта)
		byte_t reg_1 : 4; ///< номер первого регистра, 4 бита
		byte_t reg_2 : 4; ///< номер второго регистра, 4 бита
		word offset; ///< смещение, 16 бит
	}
	command; ///< команда
	byte_t raw[4]; ///< "сырые" данные
};
#pragma pack(pop)

#endif //OOP_COMMON_H
