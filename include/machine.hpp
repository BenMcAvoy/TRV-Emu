#include "cpu.hpp"

// The Machine class is the main class that will be used to run the emulator.
// It contains the necessary virtual hardware components, such as the CPU and memory.
// This will also operate as a clock, stepping through the CPU and memory.
// The Machine class will be responsible for loading the program into memory and starting the CPU.
class Machine {
private:
    CPU cpu;

public:
    // NOTE: Only to be used for display purposes
    Memory memory;

    Machine();
    ~Machine();

    void step();
};
