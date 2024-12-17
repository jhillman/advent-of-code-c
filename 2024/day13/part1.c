/* Day 13, part 1 = 29598 */

#include "../../aoc.h"
#include "machines.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    long long answer = totalCost(aoc.input, 0);

    print(aoc, LONG_LONG, &answer);

    return 0;
}