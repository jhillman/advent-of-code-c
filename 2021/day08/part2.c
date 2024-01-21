/* Day 8, part 2 = 1016804 */

#include "../../aoc.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int compare(const void *a, const void *b) {
    return *(char *)a - *(char *)b;
}

struct Signal {
    char wires[8];
    bool wireMap[8];
    int wireCount;
    int digit;
};

bool hasWire(struct Signal signal, char wire) {
    return signal.wireMap[wire - 'a'];
}

bool sharesWires(struct Signal signal1, struct Signal signal2) {
    bool shared = true;

    for (char wire = 'a'; shared && wire <= 'g'; wire++) {
        if (hasWire(signal1, wire)) {
            shared = hasWire(signal2, wire);
        } 
    }

    return shared;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        struct Signal signals[10];
        char outputs[4][8];
        struct Signal digitSignalMap[10];
        char ch;

        while (!feof(inputFile)) {
            for (int i = 0; i < 10; i++) {
                fscanf(inputFile, "%s%c", signals[i].wires, &ch);

                signals[i].wireCount = strlen(signals[i].wires);

                qsort(signals[i].wires, signals[i].wireCount, sizeof(char), compare);

                switch (signals[i].wireCount) {
                    case 2:
                        signals[i].digit = 1;
                        break;
                    case 3:
                        signals[i].digit = 7;
                        break;
                    case 4:
                        signals[i].digit = 4;
                        break;
                    case 7:
                        signals[i].digit = 8;
                        break;
                    default:
                        signals[i].digit = -1;
                }

                for (char wire = 'a'; wire <= 'g'; wire++) {
                    signals[i].wireMap[wire - 'a'] = strchr(signals[i].wires, wire) != NULL;
                }

                if (signals[i].digit > -1) {
                    digitSignalMap[signals[i].digit] = signals[i];
                }
            }

            // |
            fscanf(inputFile, "%s%c", *outputs, &ch);

            for (int i = 0; i < 4; i++) {
                fscanf(inputFile, "%s%c", outputs[i], &ch);

                qsort(outputs[i], strlen(outputs[i]), sizeof(char), compare);
            }

            // find 3
            for (int i = 0; i < 10; i++) {
                if (signals[i].digit == -1 && signals[i].wireCount == 5 && sharesWires(digitSignalMap[1], signals[i]) && sharesWires(digitSignalMap[7], signals[i])) {
                    signals[i].digit = 3;
                    digitSignalMap[3] = signals[i];

                    break;
                }
            }

            // find 9
            for (int i = 0; i < 10; i++) {
                if (signals[i].digit == -1 && signals[i].wireCount == 6 && sharesWires(digitSignalMap[3], signals[i])) {
                    signals[i].digit = 9;
                    digitSignalMap[9] = signals[i];

                    break;
                }
            }

            // find 5
            for (int i = 0; i < 10; i++) {
                if (signals[i].digit == -1 && signals[i].wireCount == 5 && sharesWires(signals[i], digitSignalMap[9])) {
                    signals[i].digit = 5;
                    digitSignalMap[5] = signals[i];

                    break;
                }
            }

            // find 0 and 6
            for (int i = 0; i < 10; i++) {
                if (signals[i].digit == -1 && signals[i].wireCount == 6) {
                    if (sharesWires(digitSignalMap[5], signals[i])) {
                        signals[i].digit = 6;
                        digitSignalMap[6] = signals[i];

                        if (digitSignalMap[0].digit == 0) {
                            break;
                        }
                    } else {
                        signals[i].digit = 0;
                        digitSignalMap[0] = signals[i];

                        if (digitSignalMap[6].digit == 6) {
                            break;
                        }
                    }
                }
            }

            // find 2
            for (int i = 0; i < 10; i++) {
                if (signals[i].digit == -1) {
                    signals[i].digit = 2;
                    digitSignalMap[2] = signals[i];

                    break;
                }
            }

            int outputValue = 0;
            int multiplier = 1000;

            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 10; j++) {
                    if (strcmp(signals[j].wires, outputs[i]) == 0) {
                        outputValue += multiplier * signals[j].digit;

                        multiplier /= 10;
                        break;
                    }
                }
            }

            answer += outputValue;

            for (int i = 0; i < 10; i++) {
                digitSignalMap[i].digit = -1;
                signals[i].digit = -1;
            }
        }

        fclose(inputFile);
    }

    print(aoc, INT, &answer);

    return 0;
}
