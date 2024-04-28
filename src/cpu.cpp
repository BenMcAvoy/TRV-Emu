#include "../include/cpu.hpp"
#include "../include/logging.hpp"

#include "../src/instruction.cpp"

#include <bitset>

void CPU::fetch() {
    // Fetch the instruction from memory
    uint32_t ins = this->memory->read32(this->pc);
    iFMT fmt = getFormat(ins);

    // Print the format
    switch (fmt) {
        case R:
            LOG_WARN("Instruction format R not implemented");
            break;
        case I:
            { // Scope for variables to allow stack allocation in a switch statement
            uint32_t opcode = ins & 0x7F; ins >>= 7;
            uint32_t rd = ins & 0x1F; ins >>= 5;
            uint32_t funct3 = ins & 0x3; ins >>= 3;
            uint32_t rs1 = ins & 0x5; ins >>= 5;
            uint32_t imm = ins;

            LOG_DEBUG("Instruction (%s): %s %02X %02X %02X %d",
                "I", std::bitset<7>(opcode).to_string().c_str(),
                rd, funct3, rs1, imm
            );

            }

            break;
        case S:
            LOG_WARN("Instruction format S not implemented");
            break;
        case B:
            LOG_WARN("Instruction format B not implemented");
            break;
        case U:
            LOG_WARN("Instruction format U not implemented");
            break;
        case J:
            LOG_WARN("Instruction format J not implemented");
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
