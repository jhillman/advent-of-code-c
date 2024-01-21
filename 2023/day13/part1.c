/* Day 13, part 1 = 33735 */

#include "../../aoc.h"
#include "mirrors.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    int answer = mirrorLocationScoreSum(aoc.input, false);

    print(aoc, INT, &answer);
}
