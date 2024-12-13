/* Day 11, part 2 = 232454623677743 */

#include "../../aoc.h"
#include "stones.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    long answer = stones(aoc.input, 75);

    print(aoc, LONG, &answer);

    return 0;
}
