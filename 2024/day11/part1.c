/* Day 11, part 1 = 194482 */

#include "../../aoc.h"
#include "stones.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    long answer = stones(aoc.input, 25);

    print(aoc, LONG, &answer);

    return 0;
}