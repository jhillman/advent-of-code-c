/* Day 11, part 1 = 9734203 */

#include "../../aoc.h"
#include "expansion.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    long answer = galaxyDistanceSum(aoc.input, 2);

    print(aoc, LONG, &answer);
}
