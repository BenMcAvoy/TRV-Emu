#include "../include/cpu.hpp"
#include "../include/logging.hpp"

#include "../src/instruction.cpp"

void CPU::fetch() {
    // Fetch the instruction from memory
    uint32_t ins = this->memory->read32(this->pc);
    iFMT fmt = getFormat(ins);

    // Print the format
    switch (fmt) {
        case R:
            LOG_INFO("Instruction format: R");
            break;
        case I:
            LOG_INFO("Instruction format: I");
            break;
        case S:
            LOG_INFO("Instruction format: S");
            break;
        case B:
            LOG_INFO("Instruction format: B");
            break;
        case U:
            LOG_INFO("Instruction format: U");
            break;
        case J:
            LOG_INFO("Instruction format: J");
            break;
    }

    // Increment the program counter
    this->pc += 4;
}

CPU::CPU(Memory* memory) {
    this->memory = memory;
    LOG_INFO("CPU initializing");
}

CPU::~CPU() {
    LOG_INFO("CPU shutting down");
    this->memory = nullptr;
}
