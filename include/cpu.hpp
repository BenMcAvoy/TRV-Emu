#ifndef CPU_HPP
#define CPU_HPP

#include "memory.hpp"

#include <cstdint>

class CPU {
    public:
        CPU();
        ~CPU();

        Memory memory;

    private:
};

#endif
