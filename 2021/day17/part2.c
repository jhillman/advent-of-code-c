/* Day 17, part 2 = 4433 */

#include "../../aoc.h"
#include "shoot.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        int targetMinX;
        int targetMaxX;
        int targetMinY;
        int targetMaxY;

        fscanf(inputFile, "target area: x=%d..%d, y=%d..%d", &targetMinX, &targetMaxX, &targetMinY, &targetMaxY);

        fclose(inputFile);

        answer = shoot(targetMinX, targetMaxX, targetMinY, targetMaxY).hitCount;
    }

    print(aoc, INT, &answer);

    return 0;
}
