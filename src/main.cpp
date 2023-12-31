#include "global.h"
#include "romLoader.h"
#include <iostream>
#include <SDL_events.h>
#include <SDL.h>
#include "RenderWindow/main_window.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/async_logger.h"
#include "spdlog/async.h"

uint8_t RAM[65536];
uint8_t *internal_mem = RAM;
ROM rom;
REGISTERS registers;

bool is_bit_set(uint8_t operand, char bit){
    return (operand & (0x1 << bit)) >> bit;
}

void power_up(){
    memset(internal_mem,0,65536);
    registers.sr = 0x34;
    registers.ac = 0,registers.x = 0,registers.y = 0;
    registers.sp = 0xFD;
    registers.rw_register_mode = 0x0;
    internal_mem[0x4017] = 0;
    internal_mem[0x4015] = 0;
    for(int i = 0x4000; i <= 0x400F; i++){
        internal_mem[i] = 0;
    }
    for(int i = 0x4010; i <= 0x4013; i++){
        internal_mem[i] = 0;
    }
    load_rom();
    registers.pc = RESET_VECTOR;
    spdlog::info("RESET_VECTOR: 0x{:X}",RESET_VECTOR);
}

void exit(){
    SDL_DestroyTexture(pixels);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    spdlog::shutdown();
    SDL_Quit();
}

void init_spdlog(){
    spdlog::init_thread_pool(8192, 1);
    auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("latestLog.txt", true);
    std::vector<spdlog::sink_ptr> sinks {stdout_sink, file_sink};
    auto async_logger = std::make_shared<spdlog::async_logger>("NESEmu", sinks.begin(), sinks.end(), spdlog::thread_pool());
    spdlog::set_default_logger(async_logger);
}

int main(int argc, char* argv[]) {
    init_spdlog();
    init_video();
    power_up();
    spdlog::info("PC REGISTER: 0x{:X}", registers.pc);
    spdlog::info("INITIAL OPCODE: 0x{:X}",internal_mem[registers.pc]);
    spdlog::set_level(spdlog::level::debug);
    SDL_Event event;
    bool quit = false;
    time_t CPU = time(NULL);
    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
        if(time(NULL) - CPU >= 1){
            registers.cycles = 0;
            CPU = time(NULL);
        }
        if(registers.cycles < 1790000) {
            spdlog::debug("EXECUTING OPCODE: 0x{:X} AT PC REGISTER: 0x{:X}", internal_mem[registers.pc], registers.pc);
            execute_opcode(internal_mem[registers.pc++]);
        }

        // ... (update pixels and rendering code here)
    }
    exit();
    return 0;
}
