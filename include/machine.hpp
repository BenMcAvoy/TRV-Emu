#include "cpu.hpp"

class Machine {
private:
    Memory memory;
    CPU cpu;

public:
    Machine();
    ~Machine();

    void step();
};
