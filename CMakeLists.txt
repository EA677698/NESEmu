cmake_minimum_required(VERSION 3.20)
project(emulator)
set(CMAKE_CXX_STANDARD 17)


add_executable(emulator src/main.cpp src/cpu.cpp src/global.h src/instructions.cpp src/apu.cpp src/ppu.cpp src/instructions.h src/ppu.h)