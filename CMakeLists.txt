cmake_minimum_required(VERSION 3.20)
project(emulator)
set(CMAKE_CXX_STANDARD 17)

set(SDL2_PATH "C:/SDL2")


include_directories(${SDL2_PATH}/include)
link_directories(${SDL2_PATH}/lib/x86)

add_executable(emulator src/main.cpp src/cpu.cpp src/global.h src/instructions.cpp
        src/apu.cpp src/ppu.cpp src/instructions.h src/ppu.h src/util.cpp src/romLoader.cpp)

target_link_libraries(emulator SDL2.lib SDL2main.lib)