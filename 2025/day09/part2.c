/* Day 9, part 2 = 1410501884 */

#include "../../aoc.h"
#include "tiles.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    long answer = largestArea(aoc.input, true);

    print(aoc, LONG, &answer);

    return 0;
}
