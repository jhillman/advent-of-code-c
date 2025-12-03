/* Day 3, part 2 = 173065202451341 */

#include "../../aoc.h"
#include "joltage.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    long answer = totalJoltage(aoc.input, 12);

    print(aoc, LONG, &answer);

    return 0;
}
