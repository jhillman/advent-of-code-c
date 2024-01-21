/* Day 9, part 2 = 942 */

#include "../../aoc.h"
#include "extrapolation.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    long answer = getExtrapolatedSum(aoc.input, PREVIOUS);

    print(aoc, LONG, &answer);
}
