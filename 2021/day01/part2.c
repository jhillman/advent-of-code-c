/* Day 1, part 2 = 1728 */

#include "../../aoc.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        int sums[3] = {0, 0, 0};
        int addendCounts[3] = {0, 0, 0};
        int measurementCount = 1;
        int measurement;
        int previousSum = 0;

        while (!feof(inputFile) && fscanf(inputFile, "%d", &measurement)) {
            for (int i = 0; i < measurementCount; i++) {
                sums[i] += measurement;

                if (++addendCounts[i] == 3) {
                    if (previousSum && sums[i] > previousSum) {
                        ++answer;
                    }

                    previousSum = sums[i];

                    sums[i] = 0;
                    addendCounts[i] = 0;
                }
            }

            if (measurementCount < 3) {
                ++measurementCount;
            }
        }

        fclose(inputFile);
    }

    print(aoc, INT, &answer);

    return 0;
}
