/* Day 17, part 1 = 286 */

#include "../../aoc.h"
#include "pocket.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct PocketDimension *pocketDimension = readPocketDimension(aoc.input, false);
    int answer = 0;

    if (pocketDimension) {
        for (int i = 0; i < 6; i++) {
            pocketDimension = cyclePocketDimension(pocketDimension);
        }

        int activeCount = activeCubeCount(pocketDimension);

        freePocketDimension(pocketDimension);

        answer = activeCount;
    }

    print(aoc, INT, &answer);

    return 0;
}
