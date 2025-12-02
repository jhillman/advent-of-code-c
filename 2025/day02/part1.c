/* Day 2, part 1 = 21898734247 */

#include "../../aoc.h"
#include "invalid.h"

int firstSequenceLength(int digits) {
    return (int)ceil((float)digits / 2.0f);
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    long answer = invalidSum(aoc.input);

    print(aoc, LONG, &answer);

    return 0;
}