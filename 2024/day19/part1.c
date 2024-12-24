/* Day 19, part 1 = 322 */

#include "../../aoc.h"
#include "towels.h"

void processTowelsPossible(long possible, long *result) {
    *result += possible ? 1 : 0;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    long answer = 0;

    processTowelPatterns(aoc.input, &answer);

    print(aoc, LONG, &answer);

    return 0;
}
