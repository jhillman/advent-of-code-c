/* Day 7, part 2 = 17921968177009 */

#include "../../aoc.h"
#include "tachyon.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    long answer = getTimes(aoc.input, true);

    print(aoc, LONG, &answer);

    return 0;
}