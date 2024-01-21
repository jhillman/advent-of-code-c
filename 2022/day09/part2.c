/* Day 9, part 2 = 2449 */

#include "../../aoc.h"
#include "rope.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = countTailPositions(aoc.input, 10);

    print(aoc, INT, &answer);

    return 0;
}