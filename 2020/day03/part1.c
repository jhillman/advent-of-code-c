/* Day 3, part 1 = 216 */

#include "../../aoc.h"
#include <string.h>

#define RIGHT 3
#define DOWN 1

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        char mapLine[40];
        char mapLineLength;
        int downPosition = 0;
        int rightPosition = 0;
        int treeCount = 0;

        while (fgets(mapLine, sizeof(mapLine), inputFile)) {
            mapLineLength = strlen(mapLine);

            if (mapLine[mapLineLength - 1] == '\n') {
                --mapLineLength;
            }

            if (rightPosition == 0 && downPosition == 0 && *mapLine == '#') {
                ++treeCount;
            }

            if (downPosition == DOWN) {
                rightPosition += RIGHT;

                if (mapLine[rightPosition % mapLineLength] == '#') {
                   ++treeCount;
                }

                downPosition = 0;
            }

            ++downPosition;
        }

        fclose(inputFile);

        answer = treeCount;
    }

    print(aoc, INT, &answer);

    return 0;
}