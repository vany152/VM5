cmake_minimum_required(VERSION 3.20)
project(VM5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS -static)

add_executable(vm5
        main.cpp
        Memory.h
        CPU.cpp CPU.h

        commands/Command.h
        commands/Arithmetic.cpp commands/Arithmetic.h
        commands/Move.cpp commands/Move.h
        commands/Jump.cpp commands/Jump.h
        commands/Compare.cpp commands/Compare.h
        commands/IO.cpp commands/IO.h
        commands/Stop.h

        common.h
        Preprocessor.cpp Preprocessor.h
        )
