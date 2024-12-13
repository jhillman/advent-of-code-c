/* Day 12, part 1 = 1473276 */

#include "../../aoc.h"
#include "fence.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = price(aoc.input, false);

    print(aoc, INT, &answer);

    return 0;
}
