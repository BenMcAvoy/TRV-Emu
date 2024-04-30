#!/usr/bin/env bash

# This script is used to assemble the assembly code
# and give you C++ compatible code which you can use
# in the VM.

set -e

riscv64-unknown-linux-gnu-as -o prog.o prog.s
OUTPUT=$(riscv64-unknown-linux-gnu-objdump -d prog.o | tail -n +8)
ASM=$(echo "$OUTPUT" | cut -d "	" -f 2)

COMMENTS=$(echo "$OUTPUT" | cut -d "	" -f 3-)

echo "const uint32_t program[] = {" | tee output.txt

for index in $(seq 1 "$(echo "$ASM" | wc -l)"); do
    line=$(echo "$ASM" | head -n "$index" | tail -n 1)
    comment=$(echo "$COMMENTS" | head -n "$index" | tail -n 1 | tr '\t' ' ' | sed 's/,/, /g')
    asm=$(echo "$line" | tr -d '[:space:]')

    echo "    0x$asm,    // $comment" | tee -a output.txt
done

echo "};" | tee -a output.txt
