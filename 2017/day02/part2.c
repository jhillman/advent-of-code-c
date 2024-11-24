/* Day 2, part 2 = 275 */

#include "../../aoc.h"
#include <stdlib.h>

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        int valueIndex = 0;
        int value;
        char c;
        int *values;
        int minimum = 0;
        int maximum = 0;
        int result;
        int sum = 0;

        while (fscanf(inputFile, "%d%c", &value, &c)) {
            ++valueIndex;

            if (c == '\n') {
                break;
            }
        }

        values = (int *)malloc(valueIndex * sizeof(int));
        valueIndex = 0;

        fseek(inputFile, 0, SEEK_SET);

        while (!feof(inputFile) && fscanf(inputFile, "%d%c", &value, &c)) {
            values[valueIndex++] = value;

            if (feof(inputFile) || c == '\n') {
                result = 0;

                for (int i = 0; !result && i < valueIndex; i++) {
                    for (int j = i + 1; !result && j < valueIndex; j++) {
                        minimum = min(values[i], values[j]);
                        maximum = max(values[i], values[j]);

                        if (maximum % minimum == 0) {
                            result = maximum / minimum;
                        }
                    }
                }

                sum += result;

                valueIndex = 0;
            }
        }

        fclose(inputFile);

        answer = sum;

        free(values);
    }

    print(aoc, INT, &answer);

    return 0;
}