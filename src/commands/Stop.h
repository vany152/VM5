#ifndef ARCHITECTURY_STOP_H
#define ARCHITECTURY_STOP_H

// включение iostream вызывает проблему при создании указателя на div() - модуль CPU.cpp
#include <cstdio>

#include "Command.h"

/**
 * @brief команда остановки процессора
 */
struct stop final : public Command
{
	virtual void operator()(CPU & cpu) { printf("stop\n"); }
};

#endif //ARCHITECTURY_STOP_H
