#include <string>

#include "../include/memory.hpp"
#include "../include/logging.hpp"

constexpr uint32_t MEMORY_SIZE = 0xFFFFFFFF;
static_assert(MEMORY_SIZE > 0, "Memory size must be greater than 0");
static_assert(MEMORY_SIZE <= 4294967296, "Memory size must be less than or equal to the 32-bit max value");

Memory::Memory() {
    memory = new uint8_t[MEMORY_SIZE];
    memset(memory, 0x0, MEMORY_SIZE);
    LOG_INFO("Memory initializing");
}

Memory::~Memory() {
    LOG_INFO("Memory deinitalizing");
    delete[] memory;
}

// NOTE: 1 is returned if the address is out of bounds
int Memory::loadBytes(uint32_t offset, uint8_t *bytes, size_t size) {
    LOG_DEBUG("Loading %zu bytes into memory at 0x%08X", size, offset);

    if (offset + (uint32_t)size > MEMORY_SIZE) {
        LOG_ERROR("Memory load out of bounds: 0x%08X", offset);
        return 1;
    }

    for (int i = 0; i < (int)size; i++)
        this->write8(offset + i, bytes[i]);

    LOG_DEBUG("Loaded %zu bytes into memory at 0x%08X", size, offset);

    return 0;
}

// NOTE: 1 is returned if the address is out of bounds
uint8_t Memory::write32(uint32_t address, uint32_t value) {
    if (address > (MEMORY_SIZE - 4)) {
        LOG_ERROR("Memory address wrote out of bounds: 0x%08X", address);
        return 1;
    }

    write8(address, value & 0xFF);
    write8(address + 1, (value >> 8) & 0xFF);
    write8(address + 2, (value >> 16) & 0xFF);
    write8(address + 3, (value >> 24) & 0xFF);

    LOG_DEBUG("Wrote 0x%08X to 0x%08X", value, address);

    return 0;
}

// NOTE: 0 is returned if the address is out of bounds
uint32_t Memory::read32(uint32_t address) {
    if (address > (MEMORY_SIZE - 4)) {
        LOG_ERROR("Memory address read out of bounds: 0x%08X", address);
        return 0;
    }

    uint32_t value = 0;
    value |= read8(address);
    value |= read8(address + 1) << 8;
    value |= read8(address + 2) << 16;
    value |= read8(address + 3) << 24;

    LOG_DEBUG("Read 0x%08X from 0x%08X", value, address);

    return value;
}

void Memory::write8(uint32_t address, uint8_t value) {
    memory[address] = value;
}

uint8_t Memory::read8(uint32_t address) {
    return memory[address];
}
