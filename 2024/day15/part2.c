/* Day 15, part 2 = 1522215 */

#include "../../aoc.h"
#include "robots.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = boxGPSSum(aoc.input, true);

    print(aoc, INT, &answer);

    return 0;
}
