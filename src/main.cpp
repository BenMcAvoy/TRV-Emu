#include <string>
#include <cstdio>

#include "memory.cpp"

int main(void) {
    Memory memory;

    memory.write32(0, 0x12345678);
    printf("0x%08X\n", memory.read32(0));
}
