/* Day 12, part 2 = 901100 */

#include "../../aoc.h"
#include "fence.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = price(aoc.input, true);

    print(aoc, INT, &answer);

    return 0;
}
