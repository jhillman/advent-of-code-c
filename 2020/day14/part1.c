/* Day 14, part 1 = 9296748256641 */

#include "../../aoc.h"
#include "computer.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Computer *computer = getComputer(aoc.input);
    long answer = 0;

    if (computer) {
        char mask[MASK_LENGTH + 1];
        long bit = 1;
        char *maskPtr;
        long value;

        long memorySum = 0;

        for (int i = 0; i < computer->instructionCount; i++) {
            switch (computer->instructions[i].type) {
                case MASK:
                    strcpy(mask, computer->instructions[i].mask);
                    break;
                case MEMORY:
                    bit = 1;
                    maskPtr = mask + MASK_LENGTH - 1;
                    value = computer->instructions[i].value;

                    while (maskPtr >= mask) {
                        if (*maskPtr == '1' && (value & bit) == 0) {
                            value += bit;
                        } else if (*maskPtr == '0' && (value & bit) == bit) {
                            value -= bit;
                        }

                        --maskPtr;
                        bit <<= 1;
                    }

                    memorySum -= computer->memory[computer->instructions[i].memoryOffset];
                    memorySum += value;

                    computer->memory[computer->instructions[i].memoryOffset] = value;
                    break;
            }
        }

        freeComputer(computer);

        answer = memorySum;
    }

    print(aoc, LONG, &answer);

    return 0;
}
