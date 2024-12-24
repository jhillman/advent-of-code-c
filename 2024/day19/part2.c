/* Day 19, part 2 = 715514563508258 */

#include "../../aoc.h"
#include "towels.h"

void processTowelsPossible(long possible, long *result) {
    *result += possible;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    long answer = 0;

    processTowelPatterns(aoc.input, &answer);

    print(aoc, LONG, &answer);

    return 0;
}
