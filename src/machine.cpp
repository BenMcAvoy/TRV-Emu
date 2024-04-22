#include "logging.hpp"

#include "machine.hpp"
#include "memory.cpp"
#include "cpu.cpp"

Machine::Machine() : cpu(&memory) {
    LOG_INFO("Machine starting");
}

Machine::~Machine() {
    LOG_INFO("Machine shutting down");
}

void Machine::step() {
    LOG_WARN("Stepping machine is not implemented");
}