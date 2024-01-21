/* Day 3, part 2 = 6708199680 */

#include "../../aoc.h"
#include <string.h>

typedef struct {
    int rightAmount;
    int downAmount;
    int rightPosition;
    int downPosition;
    long treeCount;
} Slope;

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    long answer = 0;

    if (inputFile) {
        char mapLine[40];
        char mapLineLength;
        Slope slopes[] = { 
            { 1, 1, 0, 0, 0 }, 
            { 3, 1, 0, 0, 0 }, 
            { 5, 1, 0, 0, 0 }, 
            { 7, 1, 0, 0, 0 }, 
            { 1, 2, 0, 0, 0 } 
        };
        int slopeCount = sizeof(slopes) / sizeof(*slopes);
        Slope *slope;

        while (fgets(mapLine, sizeof(mapLine), inputFile)) {
            mapLine[strcspn(mapLine, "\n")] = 0;
            mapLineLength = strlen(mapLine);

            for (int i = 0; i < slopeCount; i++) {        
                slope = &slopes[i];

                if (slope->rightPosition == 0 && slope->downPosition == 0 && *mapLine == '#') {
                    ++slope->treeCount;
                }

                if (slope->downPosition == slope->downAmount) {
                    slope->rightPosition += slope->rightAmount;

                    if (mapLine[slope->rightPosition % mapLineLength] == '#') {
                       ++slope->treeCount;
                    }

                    slope->downPosition = 0;
                }

                ++slope->downPosition;
            }
        }

        fclose(inputFile);

        long totalTreeCount = 1;

        for (int i = 0; i < slopeCount; i++) {
            totalTreeCount *= slopes[i].treeCount;
        }

        answer = totalTreeCount;
    }

    print(aoc, LONG, &answer);

    return 0;
}