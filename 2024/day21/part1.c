/* Day 21, part 1 = 162740 */

#include "../../aoc.h"
#include "keypads.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    long answer = getComplexitySum(aoc.input, 2);

    print(aoc, LONG, &answer);

    return 0;
}
