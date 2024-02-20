/* Day 9, part 1 = 400493 */

#include "marbles.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    long answer = play(aoc.input, 1);

    print(aoc, LONG, &answer);

    return 0;
}
