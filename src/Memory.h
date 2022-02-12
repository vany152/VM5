#ifndef ARCHITECTURY_MEMORY_H
#define ARCHITECTURY_MEMORY_H

#include <cstdint>
#include "common.h"

class Memory final
{
	/**
	 * размерность адреса - 16 бит => максимальный размер - 65536 ячейки
	 * тип памяти - байтовая
	 */
	byte_t * memory;
	
public:
	Memory() { memory = new byte_t[65536]; }
	~Memory() { delete [] memory; }
	
	byte_t & operator[](const address_t & address) { return memory[address]; }
};

#endif //ARCHITECTURY_MEMORY_H
