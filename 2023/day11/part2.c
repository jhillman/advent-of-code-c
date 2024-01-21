/* Day 11, part 2 = 568914596391 */

#include "../../aoc.h"
#include "expansion.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    long answer = galaxyDistanceSum(aoc.input, 1000000);

    print(aoc, LONG, &answer);
}