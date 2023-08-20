cmake_minimum_required(VERSION 3.20)
project(emulator)
set(CMAKE_CXX_STANDARD 17)

set(SDL2_PATH "C:/SDL2")


include_directories(${SDL2_PATH}/include)
link_directories(${SDL2_PATH}/lib/x86)

include_directories(${PROJECT_SOURCE_DIR}/libs/spdlog/include)

add_executable(emulator src/main.cpp src/global.h src/CPU/instructions.cpp
        src/apu.cpp src/ppu.cpp src/CPU/instructions.h src/ppu.h src/util.cpp src/romLoader.cpp src/mappers/mapper.cpp src/mappers/mapper.h src/mappers/mapper0.cpp src/mappers/mapper0.h src/CPU/opcode_interpreter.cpp src/CPU/cpu.h src/romLoader.h)

target_link_libraries(emulator SDL2.lib SDL2main.lib)