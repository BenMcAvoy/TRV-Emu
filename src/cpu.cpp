#include "../include/cpu.hpp"
#include "../include/memory.hpp"
#include "../include/logging.hpp"

CPU::CPU() {
    LOG_INFO("CPU initializing");
}

CPU::~CPU() {
    LOG_INFO("CPU shutting down");
}