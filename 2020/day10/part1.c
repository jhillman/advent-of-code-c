/* Day 10, part 1 = 1820 */

#include "../../aoc.h"
#include <stdlib.h>

int compare(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

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

        int differencesOf1 = 0;
        int differencesOf3 = 0;

        for (int i = 1; i < adapterCount; i++) {
            if (adapters[i] - adapters[i - 1] == 3) {
                ++differencesOf3;
            } else {
                ++differencesOf1;
            }
        }

        answer = differencesOf1 * differencesOf3;
    }

    print(aoc, INT, &answer);

    return 0;
}
