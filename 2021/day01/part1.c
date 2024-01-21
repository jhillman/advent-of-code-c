/* Day 1, part 1 = 1688 */

#include "../../aoc.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        int previousMeasurement = 0;
        int measurement;

        while (!feof(inputFile) && fscanf(inputFile, "%d", &measurement)) {
            if (previousMeasurement && measurement > previousMeasurement) {
                ++answer;
            }

            previousMeasurement = measurement;
        }

        fclose(inputFile);
    }

    print(aoc, INT, &answer);

    return 0;
}
