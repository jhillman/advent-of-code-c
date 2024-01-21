/* Day 12, part 1 = 7169 */

#include "../../aoc.h"
#include "springs.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    long answer = arrangementCountSum(aoc.input, 1);

    print(aoc, LONG, &answer);

    return 0;
}

