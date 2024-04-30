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

for line in $ASM; do
    asm=$(echo "$line" | tr -d '[:space:]')

    # Get comment and also replace tabs with spaces and also add a space after commas
    comment=$(echo "$COMMENTS" | head -n 1 | tr '\t' ' ' | sed 's/,/, /g')

    echo "    0x$asm,    // $comment" | tee -a output.txt
done

echo "};" | tee -a output.txt
