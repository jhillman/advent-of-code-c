/* Day 8, part 2 = 1352 */

#include "../../aoc.h"
#include "antinodes.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = getAntinodeCount(aoc.input, true);

    print(aoc, INT, &answer);

    return 0;
}
