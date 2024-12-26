cmake_minimum_required(VERSION 3.27)
project(emulator)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog
        GIT_TAG        7c02e204c92545f869e2f04edaab1f19fe8b19fd # Release 1.13.0
)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL
        GIT_TAG        ffa78e6bead23e2ba3adf8ec2367ff2218d4343c # Release 2.28.0
)
FetchContent_MakeAvailable(SDL2)

add_executable(emulator src/main.cpp src/global.h src/CPU/instructions.cpp
        src/apu.cpp src/PPU/ppu.cpp src/PPU/ppu.h src/romLoader.cpp src/mappers/mapper.cpp src/mappers/mapper.h src/mappers/mapper0.cpp src/mappers/mapper0.h src/CPU/opcode_interpreter.cpp src/CPU/cpu.h src/CPU/cpu.cpp src/romLoader.h
        src/RenderWindow/main_window.h
        src/RenderWindow/main_window.cpp)

target_link_libraries(emulator PRIVATE spdlog::spdlog SDL2::SDL2 SDL2::SDL2main)
