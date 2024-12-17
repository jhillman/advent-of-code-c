/* Day 15, part 1 = 1499739 */

#include "../../aoc.h"
#include "robots.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = boxGPSSum(aoc.input, false);

    print(aoc, INT, &answer);

    return 0;
}
