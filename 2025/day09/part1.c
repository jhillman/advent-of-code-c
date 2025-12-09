/* Day 9, part 1 = 4777816465 */

#include "../../aoc.h"
#include "tiles.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    long answer = largestArea(aoc.input, false);

    print(aoc, LONG, &answer);

    return 0;
}

