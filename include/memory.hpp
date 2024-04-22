#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <cstdint>

// Memory class (32 bit)
// This class will be used to store the memory of the machine.
// This will be used to store the program and data memory.
//
// It can store up to 4GB of memory as no memory bank switching is implemented
// and the 32 bit address space is all used up at that point.
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
