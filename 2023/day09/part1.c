/* Day 9, part 1 = 2175229206 */

#include "../../aoc.h"
#include "extrapolation.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    long answer = getExtrapolatedSum(aoc.input, NEXT);

    print(aoc, LONG, &answer);
}
