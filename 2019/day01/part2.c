/* Day 1, part 2 = 5194864 */

#include "../../aoc.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        int mass;
        int totalFuel = 0;

        while (!feof(inputFile) && fscanf(inputFile, "%d", &mass)) {
            int fuel = (mass / 3) - 2;

            while (fuel > 0) {
                totalFuel += fuel;
                
                fuel = (fuel / 3) - 2;
            }
        }

        fclose(inputFile);

        answer = totalFuel;
    }

    print(aoc, INT, &answer);

    return 0;
}