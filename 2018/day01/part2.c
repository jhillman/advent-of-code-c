/* Day 1, part 2 = 77666 */

#include "../../aoc.h"
#include <stdlib.h>
#include <stdbool.h>

struct Frequencies {
    int highest;
    int lowest;
    bool *positives;
    bool *negatives;
};

bool frequencySeen(struct Frequencies *frequencies, int frequency) {
    bool **array;
    int *limit;
    bool seen = false;

    if (frequency >= 0) {
        array = &frequencies->positives;
        limit = &frequencies->highest;
    } else {
        array = &frequencies->negatives;
        limit = &frequencies->lowest;

        frequency *= -1;
    }

    if (frequency > *limit) {
        int previousLimit = *limit;
        *limit = frequency + 100;

        *array = (bool *)realloc(*array, *limit * sizeof(bool));
        memset((*array) + previousLimit, false, *limit - previousLimit);
    } else {
        seen = (*array)[frequency];
    }
    
    (*array)[frequency] = true;

    return seen;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        int *values = NULL;
        int valueCount = 0;
        int valueIndex = 0;
        int value;
        int frequency = 0;
        int repeated = 0;

        struct Frequencies *frequencies = (struct Frequencies *)calloc(1, sizeof(struct Frequencies));

        while (!repeated) {
            if (feof(inputFile)) {
                value = values[valueIndex++ % valueCount];
            } else {
                fscanf(inputFile, "%d", &value);
                values = (int *)realloc(values, (valueCount + 1) * sizeof(int));
                values[valueCount++] = value;
            }

            frequency += value;

            if (frequencySeen(frequencies, frequency)) {
                repeated = frequency;
            }
        }

        fclose(inputFile);

        answer = repeated;

        free(values);

        free(frequencies->positives);
        free(frequencies->negatives);
        free(frequencies);
    }

    print(aoc, INT, &answer);

    return 0;
}
