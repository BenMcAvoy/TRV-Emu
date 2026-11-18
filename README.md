# TRV-Emu

TRV-Emu is an experimental RISC-V emulator written in C++. I built it as a way to explore instruction decoding, memory, CPU state and the tools that make an emulator easier to inspect while it runs.

The project is still an early prototype. It currently fetches 32-bit words and decodes a subset of RV32I instructions for logging; it does not yet execute the full RV32I instruction set. The bundled program is a small hard-coded example.

## What you can explore

- A C++ model of a machine, CPU and memory.
- Instruction format and opcode decoding for a growing subset of RV32I.
- An ImGui interface with CPU registers, logs and a memory editor.
- Manual stepping, an optional timed clock, and byte-level memory read/write controls.

## Build

You will need a C++20 compiler, CMake, OpenGL development libraries and a working Git installation. CMake fetches GLFW and Dear ImGui dependencies during configuration.

```sh
cmake -S . -B build
cmake --build build
```

Run the `TRV-Emu` executable produced in the build directory. The interface opens with a sample program loaded; use **Step** or enable the clock to advance the CPU and watch the registers and log.

## Current scope

This is a learning project rather than a complete RISC-V implementation. Decoding and execution are incomplete, and the current interface is primarily for inspecting the prototype. Contributions and bug reports are welcome through GitHub issues.
