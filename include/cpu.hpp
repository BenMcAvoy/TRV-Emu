#ifndef CPU_HPP
#define CPU_HPP

#include "memory.hpp"

class CPU {
    public:
        CPU(Memory* memory);
        ~CPU();

    private:
        Memory* memory;
};

#endif
