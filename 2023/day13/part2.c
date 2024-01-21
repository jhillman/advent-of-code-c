/* Day 13, part 2 = 38063 */

#include "../../aoc.h"
#include "mirrors.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    int answer = mirrorLocationScoreSum(aoc.input, true);

    print(aoc, INT, &answer);
}
