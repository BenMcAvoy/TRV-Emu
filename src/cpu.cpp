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

void CPU::decode(uint32_t ins) {
    iFMT fmt = getFormat(ins);

    // Print the format
    switch (fmt) {
        case R:
            { // Scope for variables to allow stack allocation in a switch statement
            uint32_t opcode = extractBits(&ins, 7);
            uint32_t rd = extractBits(&ins, 5);
            uint32_t funct3 = extractBits(&ins, 3);
            uint32_t rs1 = extractBits(&ins, 5);
            uint32_t rs2 = extractBits(&ins, 5);
            uint32_t funct7 = extractBits(&ins, 7);

            // Combination: funct7 | funct3
            uint32_t funct = (funct7 << 3) | funct3;

            LOG_DEBUG("Instruction (%s): %s %02X %02X %02X %02X %02X",
                "R", std::bitset<7>(opcode).to_string().c_str(),
                rd, funct3, rs1, rs2, funct7
            );

            switch (funct) {
                case 0x0: // 0x0 0x0
                    LOG_DEBUG("ADD");
                    break;
                case (0x20 << 3): // 0x20 0x0
                    LOG_DEBUG("SUB");
                    break;
                case 0x4: // 0x0 0x4
                    LOG_DEBUG("XOR");
                    break;
                case 0x6: // 0x0 0x6
                    LOG_DEBUG("OR");
                    break;
                case 0x7: // 0x0 0x7
                    LOG_DEBUG("AND");
                    break;
                case 0x1: // 0x0 0x1
                    LOG_DEBUG("SLL");
                    break;
                case 0x5: // 0x0 0x5
                    LOG_DEBUG("SRL");
                    break;
                case (0x20 << 3) | 0x5: // 0x20 0x5
                    LOG_DEBUG("SRA");
                    break;
                case 0x2: // 0x0 0x2
                    LOG_DEBUG("SLT");
                    break;
                case 0x3: // 0x0 0x3
                    LOG_DEBUG("SLTU");
                    break;
                default:
                    LOG_WARN("Unknown instruction with funct %02X", funct);
            }

            } // End of scope for variables

            break;
        case I:
            { // Scope for variables to allow stack allocation in a switch statement
            uint32_t opcode = extractBits(&ins, 7);
            uint32_t rd = extractBits(&ins, 5);
            uint32_t funct3 = extractBits(&ins, 3);
            uint32_t rs1 = extractBits(&ins, 5);
            uint32_t imm = extractBits(&ins, 12);

            LOG_DEBUG("Instruction (%s): %s %02X %02X %02X %d",
                "I", std::bitset<7>(opcode).to_string().c_str(),
                rd, funct3, rs1, imm
            );

            uint32_t imm_breakout = imm & 0xFE; // Get [5:11] bits

            if (opcode == 0b00000011) {
                switch (funct3) {
                    case 0x0:
                        LOG_DEBUG("LB");
                        break;
                    case 0x1:
                        LOG_DEBUG("LH");
                        break;
                    case 0x2:
                        LOG_DEBUG("LW");
                        break;
                    case 0x4:
                        LOG_DEBUG("LBU");
                        break;
                    case 0x5:
                        LOG_DEBUG("LHU");
                        break;
                }

                return;
            }

            if (opcode == 0b01100011 && funct3 == 0x0) {
                LOG_DEBUG("LW");
                return;
            }

            if (opcode == 0b01110011) {
                switch (funct3) {
                    case 0x0:
                        LOG_DEBUG("ECALL");
                        break;
                    case 0x1:
                        LOG_DEBUG("EBREAK");
                        break;
                    default:
                        LOG_WARN("Unknown instruction with funct3 %02X", funct3);
                }

                return;
            }

            switch (funct3) {
                case 0x0: // 0x0
                    LOG_DEBUG("ADDI");
                    break;
                case 0x4: // 0x4
                    LOG_DEBUG("XORI");
                    break;
                case 0x6: // 0x6
                    LOG_DEBUG("ORI");
                    break;
                case 0x7: // 0x7
                    LOG_DEBUG("ANDI");
                    break;
                case 0x1: // 0x1
                    if (imm_breakout == 0x0)
                        LOG_DEBUG("SLLI");

                    break;
                case 0x5: // 0x5
                    if (imm_breakout == 0x0)
                        LOG_DEBUG("SRLI");

                    if (imm_breakout == 0x20)
                        LOG_DEBUG("SRAI");

                    break;
                case 0x2: // 0x2
                    LOG_DEBUG("SLTI");
                    break;
                case 0x3: // 0x3
                    LOG_DEBUG("SLTIU");
                    break;
                default:
                    LOG_WARN("Unknown instruction with funct3 %02X", funct3);
            }

            } // End of scope for variables
            break;
        default:
            LOG_WARN("Unimplemented instruction format %d", fmt);
    }
}

// TODO: Move decoding to separate function
void CPU::fetch() {
    // Fetch the instruction from memory
    uint32_t ins = this->memory->read32(this->pc);
    this->decode(ins);

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
