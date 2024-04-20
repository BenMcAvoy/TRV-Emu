#include "../include/memory.hpp"

Memory::Memory() {
    memory = new uint8_t[1024 * 1024];
}

Memory::~Memory() {
    delete[] memory;
}

void Memory::write32(uint32_t address, uint32_t value) {
    write8(address, value & 0xFF);
    write8(address + 1, (value >> 8) & 0xFF);
    write8(address + 2, (value >> 16) & 0xFF);
    write8(address + 3, (value >> 24) & 0xFF);
}

uint32_t Memory::read32(uint32_t address) {
    uint32_t value = 0;
    value |= read8(address);
    value |= read8(address + 1) << 8;
    value |= read8(address + 2) << 16;
    value |= read8(address + 3) << 24;
    return value;
}

void Memory::write8(uint32_t address, uint8_t value) {
    memory[address] = value;
}

uint8_t Memory::read8(uint32_t address) {
    return memory[address];
}
