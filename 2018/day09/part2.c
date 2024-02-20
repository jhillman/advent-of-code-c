/* Day 9, part 2 = 3338341690 */

#include "marbles.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    long answer = play(aoc.input, 100);

    print(aoc, LONG, &answer);

    return 0;
}

