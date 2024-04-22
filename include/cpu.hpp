#ifndef CPU_HPP
#define CPU_HPP

#include "memory.hpp"

// TODO: Implement FDE cycle
// This should be done in `cpu.cpp`
class CPU {
    public:
        CPU(Memory* memory);
        ~CPU();

    private:
        Memory* memory;
};

#endif
