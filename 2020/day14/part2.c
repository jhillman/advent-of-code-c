/* Day 14, part 2 = 4877695371685 */

#include "../../aoc.h"
#include "computer.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Computer *computer = getComputer(aoc.input);
    long answer = 0;

    if (computer) {
        char mask[MASK_LENGTH + 1];
        long bit;
        long floatingBit;
        long floatingBitCount;
        long floatingBits;
        char *maskPtr;
        long memoryOffset;
        long value;

        long memorySum = 0;

        for (int i = 0; i < computer->instructionCount; i++) {
            switch (computer->instructions[i].type) {
                case MASK:
                    strcpy(mask, computer->instructions[i].mask);
                    break;
                case MEMORY:
                    bit = 1;
                    floatingBit = 1;
                    floatingBitCount = 0;
                    floatingBits = 0;
                    maskPtr = mask + MASK_LENGTH - 1;
                    memoryOffset = computer->instructions[i].memoryOffset;
                    value = computer->instructions[i].value;

                    while (maskPtr >= mask) {
                        if (*maskPtr == '1' && (memoryOffset & bit) == 0) {
                            memoryOffset += bit;
                        } else if (*maskPtr == 'X') {
                            ++floatingBitCount;
                            floatingBits += bit;

                            if ((memoryOffset & bit) == bit) {
                                memoryOffset -= bit;
                            }
                        }

                        --maskPtr;
                        bit <<= 1;
                    }

                    long floatingBitOptions = 1LL << floatingBitCount;

                    for (int j = 0; j < floatingBitOptions; j++) {
                        long long floatingMemoryOffset = memoryOffset;
                        bit = 1;
                        floatingBit = 1;

                        for (int k = 0; k < floatingBitCount; k++) {
                            while ((bit & floatingBits) != bit) {
                                bit <<= 1;
                            }

                            if ((j & floatingBit) == floatingBit) {
                                floatingMemoryOffset += bit;
                            }

                            bit <<= 1;
                            floatingBit <<= 1;
                        }

                        memorySum -= computer->memory[floatingMemoryOffset];
                        memorySum += value;

                        computer->memory[floatingMemoryOffset] = value;
                    }
                    break;
            }
        }

        freeComputer(computer);

        answer = memorySum;
    }

    print(aoc, LONG, &answer);

    return 0;
}
