#include <string>
#include <cstdio>

#include "cpu.cpp"
#include "memory.cpp"

int main(void) {
    CPU cpu;

    cpu.memory.write32(0xFFFFFFFB, 0x12345678);
    (void)cpu.memory.read32(0xFFFFFFFB);

    return 0;
}
