/* Day 12, part 2 = 1738259948652 */

#include "../../aoc.h"
#include "springs.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    long answer = arrangementCountSum(aoc.input, 5);

    print(aoc, LONG, &answer);

    return 0;
}

