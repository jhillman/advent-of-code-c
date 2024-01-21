/* Day 17, part 1 = 1602881844347 */

#include "../../aoc.h"
#include "cavern.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Cavern *cavern = getCavern(aoc.input);
    long answer = 0;

    if (cavern) {
        long targetRockCount = 1000000000000;
        int cavernStateCapacity = 1000;
        struct CavernState *cavernStates = (struct CavernState *)malloc(cavernStateCapacity * sizeof(struct CavernState));
        int cavernStateCount = 0;
        int cavernStateIndex;

        while (cavern->rockCount < targetRockCount) {
            dropRocks(cavern, 1);

            struct CavernState cavernState = { cavern->top, cavern->rockCount, cavern->rockType, cavern->gustIndex, cavern->cavern[cavern->top] };

            if ((cavernStateIndex = binarySearchStates(cavernState, cavernStates, 0, cavernStateCount - 1)) != -1) {
                long patternHeight = cavern->top - cavernStates[cavernStateIndex].cavernTop;
                long patternRockCount = cavern->rockCount - cavernStates[cavernStateIndex].rockCount;
                long patternCycles = (targetRockCount - cavernStates[cavernStateIndex].rockCount) / patternRockCount;
                long remainingRocks = (targetRockCount - cavernStates[cavernStateIndex].rockCount) - (patternCycles * patternRockCount);

                dropRocks(cavern, remainingRocks);

                answer = cavern->top + ((patternCycles - 1) * patternHeight);

                cavern->rockCount = targetRockCount;
            } else {
                cavernStates[cavernStateCount++] = cavernState;
                qsort(cavernStates, cavernStateCount, sizeof(struct CavernState), compare);
            }

            if (cavernStateCount == cavernStateCapacity) {
                cavernStateCapacity += 1000;

                cavernStates = (struct CavernState *)realloc(cavernStates, cavernStateCapacity * sizeof(struct CavernState));
            }
        }

        freeCavern(cavern);
    }

    print(aoc, LONG, &answer);

    return 0;
}
