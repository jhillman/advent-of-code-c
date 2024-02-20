/* Day 16, part 1 = 607 */

#include "opcodes.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        int before[4];
        int values[4];
        int after[4];
        int test[4];

        while (fscanf(inputFile, "Before: [%d, %d, %d, %d]\n", before, before + 1, before + 2, before + 3) == 4) {
            fscanf(inputFile, "%d %d %d %d\n", values, values + 1, values + 2, values + 3);
            fscanf(inputFile, "After: [%d, %d, %d, %d]\n", after, after + 1, after + 2, after + 3);

            int possible = 0;

            for (int opcode = 0; opcode < 16; opcode++) {
                for (int r = 0; r < 4; r++) {
                    test[r] = before[r];
                }

                operation(test, opcode, values[1], values[2], values[3]);

                if (devicesMatch(test, after)) {
                    ++possible;
                }
            }

            answer += possible >= 3 ? 1 : 0;
        }

        fclose(inputFile);
    }

    print(aoc, INT, &answer);

    return 0;
}
