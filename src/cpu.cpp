#include "../include/cpu.hpp"
#include "../include/logging.hpp"

#include "../src/instruction.cpp"

#include <bitset>

inline static uint32_t extractBits(uint32_t *ins, int amount) {
    uint32_t mask = (1 << amount) - 1;
    uint32_t value = *ins & mask;
    *ins >>= amount;
    return value;
}

// TODO: Move decoding to separate function
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
            uint32_t opcode = extractBits(&ins, 7);
            uint32_t rd = extractBits(&ins, 5);
            uint32_t funct3 = extractBits(&ins, 3);
            uint32_t rs1 = extractBits(&ins, 5);
            uint32_t imm = extractBits(&ins, 12);

            /* uint32_t rd = ins & 0x1F; ins >>= 5; */
            /* uint32_t funct3 = ins & 0x3; ins >>= 3; */
            /* uint32_t rs1 = ins & 0x5; ins >>= 5; */
            /* uint32_t imm = ins; */

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

    this->zero = 0;
    this->ra = 0;
    this->sp = 0;
    this->gp = 0;
    this->tp = 0;
    this->t0 = 0;
    this->t1 = 0;
    this->t2 = 0;
    this->s0 = 0;
    this->fp = 0;
    this->s1 = 0;
    this->a0 = 0;
    this->a1 = 0;
    this->a2 = 0;
    this->a3 = 0;
    this->a4 = 0;
    this->a5 = 0;
    this->a6 = 0;
    this->a7 = 0;
    this->s2 = 0;
    this->s3 = 0;
    this->s4 = 0;
    this->s5 = 0;
    this->s6 = 0;
    this->s7 = 0;
    this->s8 = 0;
    this->s9 = 0;
    this->s10 = 0;
    this->s11 = 0;
    this->t3 = 0;
    this->t4 = 0;
    this->t5 = 0;
    this->t6 = 0;

    LOG_INFO("CPU initializing");
}

CPU::~CPU() {
    LOG_INFO("CPU shutting down");
    this->memory = nullptr;
}
