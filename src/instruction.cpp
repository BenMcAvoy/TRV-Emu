#ifndef INSTRUCTION_CPP
#define INSTRUCTION_CPP

#include <cstdint>

enum iFMT {
    R, I, S, B, U, J
};

iFMT getFormat(uint32_t ins) {
    uint8_t opcode = ins & 0x7F;

    switch (opcode) {
        case 0b00110011: // R FMT
            return R;
        case 0b00010011:
        case 0b00000011:
        case 0b01110011:
        case 0b01100111: // I FMT
            return I;
        case 0b00100011: // S FMT
            return S;
        case 0b01100011: // B FMT
            return B;
        case 0b01101111: // J FMT
            return J;
        case 0b00110111:
        case 0b00010111: // U FMT
            return U;
        default:
            LOG_WARN("Unknown instruction format %02X", opcode);
            return R;
    }
}

#endif