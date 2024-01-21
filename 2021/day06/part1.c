/* Day 6, part 1 = 372300 */

#include "../../aoc.h"
#include "lanternfish.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    long answer = simulate(aoc.input, 80);

    print(aoc, INT, &answer);

    return 0;
}
