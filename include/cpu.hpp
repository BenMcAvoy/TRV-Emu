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

        void fetch(); // Fetches the instruction from memory.
        void decode(uint32_t ins); // Decodes the instruction.

        // Registers
        uint32_t x0  = 0; // Zero constant
        uint32_t x1  = 0; // Return address
        uint32_t x2  = 0; // Stack pointer
        uint32_t x3  = 0; // Global pointer
        uint32_t x4  = 0; // Thread pointer
        uint32_t x5  = 0; // Temporaries
        uint32_t x6  = 0; // Temporaries
        uint32_t x7  = 0; // Temporaries
        uint32_t x8  = 0; // Saved / Frame pointer
        uint32_t x9  = 0; // Saved register
        uint32_t x10 = 0; // Function arguments / Return values
        uint32_t x11 = 0; // Function arguments / Return values
        uint32_t x12 = 0; // Function arguments
        uint32_t x13 = 0; // Function arguments
        uint32_t x14 = 0; // Function arguments
        uint32_t x15 = 0; // Function arguments
        uint32_t x16 = 0; // Temporaries
        uint32_t x17 = 0; // Temporaries
        uint32_t x18 = 0; // Saved registers
        uint32_t x19 = 0; // Saved registers
        uint32_t x20 = 0; // Saved registers
        uint32_t x21 = 0; // Saved registers
        uint32_t x22 = 0; // Saved registers
        uint32_t x23 = 0; // Saved registers
        uint32_t x24 = 0; // Saved registers
        uint32_t x25 = 0; // Saved registers
        uint32_t x26 = 0; // Saved registers
        uint32_t x27 = 0; // Saved registers
        uint32_t x28 = 0; // Temporaries
        uint32_t x29 = 0; // Temporaries
        uint32_t x30 = 0; // Temporaries
        uint32_t x31 = 0; // Temporaries

        uint32_t *zero = &x0;
        uint32_t *ra   = &x1;
        uint32_t *sp   = &x2;
        uint32_t *gp   = &x3;
        uint32_t *tp   = &x4;
        uint32_t *t0   = &x5;
        uint32_t *t1   = &x6;
        uint32_t *t2   = &x7;
        uint32_t *s0   = &x8;
        uint32_t *fp   = &x8;
        uint32_t *s1   = &x9;
        uint32_t *a0   = &x10;
        uint32_t *a1   = &x11;
        uint32_t *a2   = &x12;
        uint32_t *a3   = &x13;
        uint32_t *a4   = &x14;
        uint32_t *a5   = &x15;
        uint32_t *a6   = &x16;
        uint32_t *a7   = &x17;
        uint32_t *s2   = &x18;
        uint32_t *s3   = &x19;
        uint32_t *s4   = &x20;
        uint32_t *s5   = &x21;
        uint32_t *s6   = &x22;
        uint32_t *s7   = &x23;
        uint32_t *s8   = &x24;
        uint32_t *s9   = &x25;
        uint32_t *s10  = &x26;
        uint32_t *s11  = &x27;
        uint32_t *t3   = &x28;
        uint32_t *t4   = &x29;
        uint32_t *t5   = &x30;
        uint32_t *t6   = &x31;

        // User-visible registers
        uint32_t pc = 0; // Program counter

        uint32_t *registers[33] = {
            zero, ra, sp, gp, tp, t0, t1, t2, fp, s1, a0, a1, a2, a3, a4, a5, a6, a7, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, t3, t4, t5, t6, &pc
        };

    private:
        Memory* memory;
};

#endif
