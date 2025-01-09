#include "global.h"
#include <SDL_events.h>
#include <SDL.h>
#include <fstream>
#include <iostream>

#include "romLoader.h"
#include "CPU/cpu.h"
#include "RenderWindow/main_window.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/async_logger.h"
#include "spdlog/async.h"
#include "cxxopts.hpp"

ROM rom;
Arguments args;
std::ofstream dump;
uint8_t * final_mem;

bool is_bit_set(uint8_t operand, char bit){
    return (operand & (0x1 << bit)) >> bit;
}

void power_up(CPU &cpu, const std::string& rom_path){
    cpu.power_up(rom_path);
}

void emulator_exit(int status){
    if(dump.is_open()){
        dump.write((char*)final_mem, 0x10000);
        dump.close();
    }
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    spdlog::shutdown();
    SDL_Quit();
    exit(status);
}

void test_cycle_counts() {

}

void init_spdlog(){
    spdlog::init_thread_pool(8192, 1);
    auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("latestLog.txt", true);
    std::vector<spdlog::sink_ptr> sinks {stdout_sink, file_sink};
    auto async_logger = std::make_shared<spdlog::async_logger>("NESEmu", sinks.begin(), sinks.end(), spdlog::thread_pool());
    set_default_logger(async_logger);
}

int main(int argc, char* argv[]) {

    init_spdlog();
    cxxopts::Options options("NES Emulator", "An emulator for the NES system");
    options.allow_unrecognised_options();
    options.add_options()
            ("r,rom", "Path to the ROM file", cxxopts::value<std::string>()->default_value(""))
            ("t,test", "Test type (e.g., nestest, blargg)", cxxopts::value<std::string>()->default_value(""))
            ("d,debug", "Enable debug mode logging", cxxopts::value<bool>()->default_value("false")->implicit_value("true"))
            ("m,dump_memory", "Dump end memory", cxxopts::value<bool>()->default_value("false")->implicit_value("true"))
            ("i,instruction_cycles", "Print instruction cycles", cxxopts::value<bool>()->default_value("false")->implicit_value("true"))
            ("p,disable_ppu", "Disable PPU", cxxopts::value<bool>()->default_value("false")->implicit_value("true"))

            ("h,help", "Print help")
    ;
    auto result = options.parse(argc, argv);
    if (result.count("help"))
    {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    uint64_t render_start = SDL_GetPerformanceCounter();;
    args.rom_path = result["rom"].as<std::string>();
    args.test_type = result["test"].as<std::string>();
    args.debug_mode = result["debug"].as<bool>();
    args.dump_memory = result["dump_memory"].as<bool>();
    args.instruction_cycles = result["instruction_cycles"].as<bool>();
    args.disable_ppu = result["disable_ppu"].as<bool>();

    if (!strcmp(args.rom_path.c_str(), "") && !args.instruction_cycles) {
        spdlog::error("No ROM file provided. Use --rom to specify the ROM path.");
        spdlog::error("Provided arguments: {}", result.arguments_string());
        exit(1);
    }
    init_video();
    PPU ppu;
    CPU cpu = args.disable_ppu ? CPU(nullptr) : CPU(&ppu);
    // CPU cpu(nullptr);

    if (args.instruction_cycles) { // internal testing for cycles
        cpu.cycles = 0;
        power_up(cpu, argv[1]);
        spdlog::info("POWER UP CYCLES: {}", cpu.cycles);
        for (int i = 0; i<0xFF; i++) {
            cpu.cycles = 1;
            cpu.execute_opcode(i);
            if (cpu.cycles > 1) {
                spdlog::info("0x{:X}: {}", i, cpu.cycles);
            }
        }
        emulator_exit(0);
    }


    power_up(cpu, args.rom_path);
    if(args.debug_mode) {
        spdlog::set_level(spdlog::level::debug);
    }
    if(args.dump_memory){
        dump.open("dump.bin", std::ios::out | std::ios::binary);
    }
    spdlog::debug("PC REGISTER: 0x{:X}", cpu.registers.pc);
    spdlog::debug("INITIAL OPCODE: 0x{:X}", cpu.mem[cpu.registers.pc]);
    SDL_Event event;
    bool quit = false;
    time_t CPU = time(NULL);
    uint8_t nestest = 0x1;
    bool blargg_initiated = false;
    CPU::Register snapshot;
    while (!quit) {
        uint64_t current_time = SDL_GetPerformanceCounter();
        double elapsed_render = (current_time - render_start) / (double)SDL_GetPerformanceFrequency();
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

        if(!strcmp(args.test_type.c_str(), "blargg")) {
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
                emulator_exit(status);
            }
        }

        if(args.debug_mode) {
            if(!strcmp(args.test_type.c_str(), "nestest")) {
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
            if(elapsed_render >= (1.0 / 60.0) && ppu.is_in_vblank()){
                render_start = current_time;
                render_frame(ppu.frame);
            }
        }

        // ... (update pixels and rendering code here)
    }
    emulator_exit(0);
}
