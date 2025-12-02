/* Day 2, part 2 = 28915664389 */

#include "../../aoc.h"
#include "invalid.h"

int firstSequenceLength(int digits) {
    return 1;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    long answer = invalidSum(aoc.input);

    print(aoc, LONG, &answer);

    return 0;
}