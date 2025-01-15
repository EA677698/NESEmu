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

unsigned char official_opcodes[] = {
    0x69, 0x65, 0x75, 0x6D, 0x7D, 0x79, 0x61, 0x71, 0x29, 0x25,
    0x35, 0x2D, 0x3D, 0x39, 0x21, 0x31, 0x0A, 0x06, 0x16, 0x0E,
    0x1E, 0x90, 0xB0, 0xF0, 0x24, 0x2C, 0x30, 0xD0, 0x10, 0x00,
    0x50, 0x70, 0x18, 0xD8, 0x58, 0xB8, 0xC9, 0xC5, 0xD5, 0xCD,
    0xDD, 0xD9, 0xC1, 0xD1, 0xE0, 0xE4, 0xEC, 0xC0, 0xC4, 0xCC,
    0xC6, 0xD6, 0xCE, 0xDE, 0xCA, 0x88, 0x49, 0x45, 0x55, 0x4D,
    0x5D, 0x59, 0x41, 0x51, 0xE6, 0xF6, 0xEE, 0xFE, 0xE8, 0xC8,
    0x4C, 0x6C, 0x20, 0xA9, 0xA5, 0xB5, 0xAD, 0xBD, 0xB9, 0xA1,
    0xB1, 0xA2, 0xA6, 0xB6, 0xAE, 0xBE, 0xA0, 0xA4, 0xB4, 0xAC,
    0xBC, 0x4A, 0x46, 0x56, 0x4E, 0x5E, 0xEA, 0x09, 0x05, 0x15,
    0x0D, 0x1D, 0x19, 0x01, 0x11, 0x48, 0x08, 0x68, 0x28, 0x2A,
    0x26, 0x36, 0x2E, 0x3E, 0x6A, 0x66, 0x76, 0x6E, 0x7E, 0x40,
    0x60, 0xE9, 0xE5, 0xF5, 0xED, 0xFD, 0xF9, 0xE1, 0xF1, 0x38,
    0xF8, 0x78, 0x85, 0x95, 0x8D, 0x9D, 0x99, 0x81, 0x91, 0x86,
    0x96, 0x8E, 0x84, 0x94, 0x8C, 0xAA, 0xA8, 0xBA, 0x8A, 0x9A, 0x98 };

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
            ("b,breakpoint", "Set instruction count breakpoint", cxxopts::value<int>()->default_value("-1"))

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
    args.breakpoint = result["breakpoint"].as<int>();

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
        for (int i = 0; i<sizeof(official_opcodes); i++) {
            cpu.registers.pc = 0;
            cpu.registers.ac = 0;
            cpu.registers.x = 0;
            cpu.registers.y = 0;
            cpu.registers.sr = 0;
            cpu.registers.sp = 0;
            memset(cpu.mem, 0, sizeof(cpu.mem));
            cpu.cycles = 1;
            cpu.execute_opcode(official_opcodes[i]);
            spdlog::info("0x{:X}: {}", official_opcodes[i], cpu.cycles);
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
            if (args.breakpoint > 0 && cpu.instruction_counter == args.breakpoint) {
                spdlog::info("Breakpoint reached at instruction count: {}", args.breakpoint);
            }
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
