/* Day 9, part 1 = 6236 */

#include "../../aoc.h"
#include "rope.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = countTailPositions(aoc.input, 2);

    print(aoc, INT, &answer);

    return 0;
}