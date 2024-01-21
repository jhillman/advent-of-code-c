/* Day 18, part 1 = 47139 */

#include "../../aoc.h"
#include "lagoon.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    long answer = lagoonVolume(aoc.input, false);

    print(aoc, LONG, &answer);

    return 0;
}
