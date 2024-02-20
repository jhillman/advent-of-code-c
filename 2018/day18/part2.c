/* Day 18, part 2 = 190836 */

#include "trees.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = resourceValue(aoc.input, 1000000000);

    print(aoc, INT, &answer);

    return 0;
}
