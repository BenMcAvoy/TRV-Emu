#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <cstdint>

// Memory class (32 bit)
class Memory {
    public:
        Memory();
        ~Memory();

        uint8_t write32(uint32_t address, uint32_t value);
        uint32_t read32(uint32_t address);

    private:
        void write8(uint32_t address, uint8_t value);
        uint8_t read8(uint32_t address);

        uint8_t *memory;
};

#endif