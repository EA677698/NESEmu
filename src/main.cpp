#include "global.h"
#include "romLoader.h"
#include <iostream>
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

void power_up(){ //TODO Complete
    registers.sr = 0x34;
    registers.ac = 0,registers.x = 0,registers.y = 0;
    registers.sp = 0xFD;
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
    spdlog::info("0xFFFD: 0x{:X}",internal_mem[0xFFFD]);
    spdlog::info("0xFFFC: 0x{:X}",internal_mem[0xFFFC]);
    spdlog::info("RESET_VECTOR: 0x{:X}",RESET_VECTOR);
}

void reset(){ //TODO Complete
    registers.sp -= 3;
    registers.sr |= 0x4;
}

int main() {
    spdlog::init_thread_pool(8192, 1);
    auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("latestLog.txt", true);
    std::vector<spdlog::sink_ptr> sinks {stdout_sink, file_sink};
    auto async_logger = std::make_shared<spdlog::async_logger>("NESEmu", sinks.begin(), sinks.end(), spdlog::thread_pool());
    spdlog::set_default_logger(async_logger);

    power_up();
    spdlog::info("PC REGISTER: 0x{:X}", registers.pc);
    spdlog::info("INITIAL OPCODE: 0x{:X}",internal_mem[registers.pc]);
    spdlog::set_level(spdlog::level::debug);
    while (1){
        spdlog::debug("EXECUTING OPCODE: 0x{:X} AT PC REGISTER: 0x{:X}",internal_mem[registers.pc],registers.pc);
        execute_opcode(internal_mem[registers.pc++]);
    }
    spdlog::shutdown();
}
