/* Day 8, part 1 = 1586 */

#include "../../aoc.h"
#include "boot.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct BootData data = readInput(aoc.input);
    int answer = 0;

    if (data.instructionCount > 0) {
        int offset = 0;
        int accumulator = 0;

        while (1) {
            if (data.instructions[offset].runCount > 0) {
                break;
            }

            data.instructions[offset].runCount++;

            switch (data.instructions[offset].operation) {
                case NOP:
                    ++offset;
                    break;
                case ACC:
                    accumulator += data.instructions[offset].argument;
                    ++offset;
                    break;
                case JMP:
                    offset += data.instructions[offset].argument;
                    break;
            }
        }

        freeData(data);

        answer = accumulator;
    }

    print(aoc, INT, &answer);

    return 0;
}
