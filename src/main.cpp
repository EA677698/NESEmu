#include "global.h"
#include <SDL_events.h>
#include <SDL.h>
#include <fstream>

#include "romLoader.h"
#include "CPU/cpu.h"
#include "RenderWindow/main_window.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/async_logger.h"
#include "spdlog/async.h"

ROM rom;
std::ofstream dump;
uint8_t * final_mem;

bool is_bit_set(uint8_t operand, char bit){
    return (operand & (0x1 << bit)) >> bit;
}

void power_up(CPU &cpu, std::string rom_path){
    cpu.power_up(rom_path);
}

void exit(){
    if(dump.is_open()){
        dump.write((char*)final_mem, 0x10000);
        dump.close();
    }
    SDL_DestroyTexture(texture);
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
    set_default_logger(async_logger);
}

int main(int argc, char* argv[]) { // [rom path] [test author] [debug mode] [dump end memory]
    uint8_t debug_mode = 0x0;
    if(argc < 2) {
        spdlog::error("No rom file provided");
        exit(1);
    }
    init_spdlog();
    init_video();
    PPU ppu;
    CPU cpu(&ppu);
    // CPU cpu(nullptr);
    power_up(cpu, argv[1]);
    debug_mode = argc > 3; // DEBUGGING/TESTING MODE
    if(argc > 3 && strcmp(argv[3], "1") == 0) {
        spdlog::set_level(spdlog::level::debug);
    }
    if(argc > 4 && strcmp(argv[4], "1") == 0){
        dump.open("dump.bin", std::ios::out | std::ios::binary);
    }
    spdlog::debug("PC REGISTER: 0x{:X}", cpu.registers.pc);
    spdlog::debug("INITIAL OPCODE: 0x{:X}", cpu.mem[cpu.registers.pc]);
    SDL_Event event;
    bool quit = false;
    time_t CPU = time(NULL);
    const char *test_type = argc > 2 ? argv[2] : "";
    uint8_t nestest = 0x1;
    bool blargg_initiated = false;
    CPU::Register snapshot;
    while (!quit) {
        final_mem = cpu.mem;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
        if(time(NULL) - CPU >= 1){
            cpu.cycles = 0;
            CPU = time(NULL);
        }

        if(!strcmp(test_type, "blargg")) {
            uint8_t status = cpu.mem[0x6000];
            if(status == 0x80){
                blargg_initiated = true;
            }
            spdlog::debug("Status: 0x{:X}", status);
            if(status != 0x80 && blargg_initiated) {
                char* res = (char*) &cpu.mem[0x6004];
                spdlog::debug("Test finished or requires reset");
                if(cpu.mem[0x6001] != 0xDE && cpu.mem[0x6002] != 0xB0 && cpu.mem[0x6003] != 0x61){
                    spdlog::warn("Unexpected validation values: 0x{:X} 0x{:X} 0x{:X}"
                                 " expected 0xDE 0xB0 0x61", cpu.mem[0x6001], cpu.mem[0x6002], cpu.mem[0x6003]);
                }
                spdlog::info("Test result: {}", res);
                exit();
                exit(status);
            }
        }

        if(debug_mode) {
            if(!strcmp(test_type, "nestest")) {
                if(nestest) {
                    spdlog::info("NESTEST: setting PC to 0xC000");
                    cpu.registers.pc = 0xC000;
                    cpu.registers.sr = 0x24;
                    spdlog::debug("INITIAL OPCODE: 0x{:X}", cpu.mem[cpu.registers.pc]);
                    nestest = 0x0;
                }
                //spdlog::debug("Status: 0x{:X}, 0x{:X}", cpu.mem[0x02], cpu.mem[0x03]);
            }

        }

        if(cpu.cycles < 1790000) {
            std::memcpy(&snapshot, &cpu.registers, sizeof(CPU::Register));
            cpu.execute_opcode(cpu.read(cpu.registers.pc++));
            // First three are the following: Address in $PC, opcode, and operand
            spdlog::debug("0x{:X}  0x{:X}  0x{:X}           A:0x{:X} X:0x{:X} Y:0x{:X} SR:0x{:X} SP:0x{:X}",
                          snapshot.pc, cpu.mem[snapshot.pc], cpu.current_operand, snapshot.ac, snapshot.x, snapshot.y, snapshot.sr, snapshot.sp);
            cpu.current_operand = 0x0;
            if(ppu.is_in_vblank()){
                render_frame(ppu.frame);
            }
        }

        // ... (update pixels and rendering code here)
    }
    exit();
    return 0;
}
