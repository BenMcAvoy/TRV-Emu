.section .text
.global _start

_start:
    li t0, 4 # Input number

    # Check if the number is even
    andi t1, t0, 1
