#include "logging.hpp"

#include "machine.hpp"
#include "memory.cpp"
#include "cpu.cpp"

Machine::Machine() : m_cpu(&memory) {
    LOG_INFO("Machine starting");
}

Machine::~Machine() {
    LOG_INFO("Machine shutting down");
}

CPU *Machine::getCPU() {
    return &m_cpu;
}

void Machine::step() {
    m_cpu.fetch();

    //LOG_WARN("Stepping machine is not implemented");
}
