/* Day 7, part 1 = 1592 */

#include "../../aoc.h"
#include "tachyon.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    long answer = getTimes(aoc.input, false);

    print(aoc, LONG, &answer);

    return 0;
}
