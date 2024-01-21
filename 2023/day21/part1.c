/* Day 21, part 1 = 3585 */

#include "../../aoc.h"
#include "farm.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Farm *farm = getFarm(aoc.input, 64);
    long answer = 0;

    if (farm) {
        answer = reachablePlots(farm, farm->start, 64, 0);

        freeFarm(farm);
    }

    print(aoc, LONG, &answer);

    return 0;
}
