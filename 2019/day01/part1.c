/* Day 1, part 1 = 3465154 */

#include "../../aoc.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        int mass;
        int totalFuel = 0;

        while (!feof(inputFile) && fscanf(inputFile, "%d", &mass)) {
            totalFuel += (mass / 3) - 2;
        }

        fclose(inputFile);

        answer = totalFuel;
    }

    print(aoc, INT, &answer);

    return 0;
}