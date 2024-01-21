/* Day 6, part 2 = 1675781200288 */

#include "../../aoc.h"
#include "lanternfish.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    long answer = simulate(aoc.input, 256);

    print(aoc, LONG, &answer);

    return 0;
}
