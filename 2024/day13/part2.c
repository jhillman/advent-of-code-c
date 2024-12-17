/* Day 13, part 2 = 93217456941970 */

#include "../../aoc.h"
#include "machines.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    long long answer = totalCost(aoc.input, 10000000000000LL);

    print(aoc, LONG_LONG, &answer);

    return 0;
}