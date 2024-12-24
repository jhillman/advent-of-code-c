/* Day 21, part 2 = 203640915832208 */

#include "../../aoc.h"
#include "keypads.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    long answer = getComplexitySum(aoc.input, 25);

    print(aoc, LONG, &answer);

    return 0;
}
