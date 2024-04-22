#ifndef CPU_HPP
#define CPU_HPP

#include "memory.hpp"

// TODO: Implement FDE cycle
// This should be done in `cpu.cpp`
//
// This is the main CPU class that will be used to interact with the memory
// and execute instructions. The CPU class will also contain the registers
// such as `x0`, `x1`, `x2`, etc. and the program counter `pc`. The CPU will
// also contain the `fetch`, `decode`, and `execute` functions that will be
// used to execute instructions and complete the FDE cycle.
class CPU {
    public:
        CPU(Memory* memory);
        ~CPU();

    private:
        Memory* memory;
};

#endif
