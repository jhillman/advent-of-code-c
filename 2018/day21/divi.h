#include "../../aoc.h"
#include "../device.h"

/*
00 #ip 3
00 seti 123 0 4
01 bani 4 456 4
02 eqri 4 72 4
03 addr 4 3 3
04 seti 0 0 3
05 seti 0 6 4
06 bori 4 65536 5
07 seti 1855046 9 4
08 bani 5 255 2
09 addr 4 2 4
10 bani 4 16777215 4
11 muli 4 65899 4
12 bani 4 16777215 4
13 gtir 256 5 2
14 addr 2 3 3
15 addi 3 1 3
16 seti 27 0 3
17 seti 0 9 2    # set r2 to 0
18 addi 2 1 1    # set r1 to r2 + 1
19 muli 1 256 1  # set r1 to r1 * 256
20 gtrr 1 5 1    # set r1 to 1 if r1 > r5
21 addr 1 3 3    # set r3 to r1 + r3
22 addi 3 1 3    # set r3 to r3 + 1
23 seti 25 5 3   # set r3 to 25
24 addi 2 1 2    # set r2 to r2 + 1
25 seti 17 0 3   # set r3 to 17
26 setr 2 7 5    # set r5 to r2
27 seti 7 9 3
28 eqrr 4 0 2
29 addr 2 3 3
30 seti 5 3 3

# instructions 17 through 26:
r2 = 0;

while (r5 > (r2 + 1) * 256) {
    r2 = r2 + 1;
}

r5 = r2;

# same as:
r5 = r5 - 256;
r5 = r5 / 256; // new DIVI integer division opcode
r5 = r5 + 1;

addi 5 -256 5
divi 5 256 5
addi 5 1 5
*/

void loopToIntegerDivision(struct Program *program) {
    int destinationRegister = program->instructions[26].c;

    for (int i = 17; i < 27; i++) {
        switch (i) {
        case 17:
            program->instructions[i] = (struct Instruction) { ADDI, destinationRegister, -256, destinationRegister };
            break;
        case 18:
            program->instructions[i] = (struct Instruction) { DIVI, destinationRegister, 256, destinationRegister };
            break;
        case 19:
            program->instructions[i] = (struct Instruction) { ADDI, destinationRegister, 1, destinationRegister };
            break;
        default:
            program->instructions[i] = (struct Instruction) { NOOP, 0, 0, 0 };
            break;
        }
    }
}
