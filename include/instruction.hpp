#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

// TODO: Create a central Instruction class
// This is because currently we only have operation codes.
// This will require research into how the RISC-V ISA works.

// RISC-V 32I Base ISA opcodes
// https://raw.githubusercontent.com/jameslzhu/riscv-card/master/riscv-card.pdf (unofficial)
enum OpCode {
    // R FMT - ALU
    ADD   = 0b0110011,
    XOR   = 0b0110011,
    OR    = 0b0110011,
    AND   = 0b0110011,
    SLL   = 0b0110011,
    SRL   = 0b0110011,
    SRA   =  0b0110011,
    SLT   = 0b0110011,
    SLTU  = 0b0110011,

    // I FMT - ALU
    ADDI  = 0b0010011,
    XORI  = 0b0010011,
    ORI   = 0b0010011,
    ANDI  = 0b0010011,
    SLLI  = 0b0010011,
    SRLI  = 0b0010011,
    SRAI  = 0b0010011,
    SLTI  = 0b0010011,
    SLTIU = 0b0010011,

    // I FMT - Load
    LB    = 0b0000011,
    LH    = 0b0000011,
    LW    = 0b0000011,
    LBU   = 0b0000011,
    LHU   = 0b0000011,

    // S FMT - Store
    SB    = 0b0100011,
    SH    = 0b0100011,
    SW    = 0b0100011,

    // B FMT - Branch
    BEQ   = 0b1100011,
    BNE   = 0b1100011,
    BLT   = 0b1100011,
    BGE   = 0b1100011,
    BLTU  = 0b1100011,
    BGEU  = 0b1100011,

    // J/I FMT - Jump
    JAL   = 0b1101111, // R
    JALR  = 0b1100111, // I

    // U FMT - LUI/AUIPC
    LUI   = 0b0110111,
    AUIPC = 0b0010111,

    // I FMT - Environment
    ECALL  = 0b1110011,
    EBREAK = 0b1110011
};

#endif
