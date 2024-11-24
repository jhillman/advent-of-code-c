/* Day 3, part 1 = 430 */

#include "../../aoc.h"
#include <stdlib.h>

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        int value;
        int odd = 1;
        int level = 0;
        int square;

        fscanf(inputFile, "%d", &value);
        fclose(inputFile);

        while (odd * odd < value) {
            level += 1;
            odd += 2;
        }

        int x = level;
        int y = level;
        int memoryValue = odd * odd;

        while (memoryValue != value) {
            if (y == level && x > -level) {
                --x;
            } else if (x == -level && y > -level) {
                --y;
            } else if (y == -level && x < level) {
                ++x;
            } else if (x == level && y < level) {
                ++y;
            } 

            --memoryValue;
        }

        answer = abs(x) + abs(y);
    }

    print(aoc, INT, &answer);

    return 0;
}
