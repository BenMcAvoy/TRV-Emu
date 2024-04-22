#include "../include/cpu.hpp"
#include "../include/logging.hpp"

CPU::CPU(Memory* memory) {
    this->memory = memory;
    LOG_INFO("CPU initializing");
}

CPU::~CPU() {
    LOG_INFO("CPU shutting down");
    this->memory = nullptr;
}
