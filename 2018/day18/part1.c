/* Day 18, part 1 = 486878 */

#include "trees.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = resourceValue(aoc.input, 10);

    print(aoc, INT, &answer);

    return 0;
}
