/* Day 10, part 2 = 3454189699072 */

#include "../../aoc.h"
#include <stdlib.h>

int compare(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    long answer = 0;

    if (inputFile) {
        char ch;
        int adapterCount = 0;

        while (!feof(inputFile)) {
            ch = fgetc(inputFile);

            if (ch == '\n') {
                ++adapterCount;
            }
        }

        if (ch != '\n' && adapterCount > 0) {
            ++adapterCount;
        }

        fseek(inputFile, 0, SEEK_SET);

        adapterCount += 2;

        int adapters[adapterCount + 2];
        int adapterIndex = 1;
        int joltage;

        while (adapterIndex < adapterCount - 1) {
            fscanf(inputFile, "%d", &joltage);

            adapters[adapterIndex++] = joltage;
        }

        fclose(inputFile);

        qsort(adapters + 1, adapterCount - 2, sizeof(int), compare);

        int maxJoltage = adapters[adapterCount - 2];
        int deviceJoltage = maxJoltage + 3;

        adapters[0] = 0;
        adapters[adapterCount - 1] = deviceJoltage;

        long arrangements[deviceJoltage + 1];

        for (int i = 0; i <= deviceJoltage; i++) {
            arrangements[i] = 0;
        }

        arrangements[0] = 1;

        for (int i = 0; i < adapterCount; i++) {
            int joltage = adapters[i];

            for (int diff = 1; diff <= 3; diff++) {
                if (joltage - diff >= 0) {
                    arrangements[joltage] += arrangements[joltage - diff];
                }
            }
        }

        answer = arrangements[maxJoltage];
    }

    print(aoc, LONG, &answer);

    return 0;
}
