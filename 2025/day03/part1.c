/* Day 3, part 1 = 17432 */

#include "../../aoc.h"
#include "joltage.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    long answer = totalJoltage(aoc.input, 2);

    print(aoc, LONG, &answer);

    return 0;
}
