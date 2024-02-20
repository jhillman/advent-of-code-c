/* Day 16, part 2 = 577 */

#include "opcodes.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        struct Tests *tests = (struct Tests *)calloc(1, sizeof(struct Tests));
        struct Test test;
        int device[4];
        int opcodes[16] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
        int matchedOpcodes = 0;
        enum Opcode opcode;
        int a;
        int b;
        int c;
        int value;

        while (fscanf(inputFile, "Before: [%d, %d, %d, %d]\n", test.before, test.before + 1, test.before + 2, test.before + 3) == 4) {
            fscanf(inputFile, "%d %d %d %d\n", test.values, test.values + 1, test.values + 2, test.values + 3);
            fscanf(inputFile, "After: [%d, %d, %d, %d]\n", test.after, test.after + 1, test.after + 2, test.after + 3);

            test.matchingOpcodes = 0;

            for (int opcode = 0; opcode < 16; opcode++) {
                for (int r = 0; r < 4; r++) {
                    device[r] = test.before[r];
                }

                operation(device, opcode, test.values[1], test.values[2], test.values[3]);

                if (devicesMatch(device, test.after)) {
                    test.matchingOpcodes |= (1 << (opcode + 1));
                }
            }

            addTest(tests, test);
        }

        while (matchedOpcodes < 16) {
            for (int i = 0; i < tests->count; i++) {
                if (matchesOneOpcode(tests->data[i])) {
                    opcode = matchingOpcode(tests->data[i]);
                    value = *tests->data[i].values;

                    opcodes[value] = opcode - 1;

                    for (int j = i + 1; j < tests->count; j++) {
                        tests->data[j].matchingOpcodes &= ~(1 << opcode);
                    }

                    ++matchedOpcodes;
                }
            }
        }

        for (int r = 0; r < 4; r++) {
            device[r] = 0;
        }

        while (fscanf(inputFile, "%d %d %d %d\n", &opcode, &a, &b, &c) == 4) {
            operation(device, opcodes[opcode], a, b, c);
        }

        fclose(inputFile);

        answer = *device;

        freeTests(tests);
    }

    print(aoc, INT, &answer);

    return 0;
}
