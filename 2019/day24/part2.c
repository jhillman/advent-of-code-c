/* Day 24, part 2 = 1902 */

#include "../../aoc.h"
#include "bugs.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = calculate(aoc.input, BUG_COUNT);

    print(aoc, INT, &answer);

    return 0;
}
