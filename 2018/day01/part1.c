/* Day 1, part 1 = 569 */

#include "../../aoc.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        int value;
        int frequency = 0;

        while (!feof(inputFile) && fscanf(inputFile, "%d", &value)) {
            frequency += value;
        }

        fclose(inputFile);

        answer = frequency;
    }

    print(aoc, INT, &answer);

    return 0;
}