/* Day 8, part 1 = 376 */

#include "../../aoc.h"
#include "antinodes.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = getAntinodeCount(aoc.input, false);

    print(aoc, INT, &answer);

    return 0;
}
