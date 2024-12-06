/* Day 15, part 1 = 567 */

#include "../../aoc.h"
#include "generators.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = matchingPairs(aoc.input, 0, 0, 40000000);

    print(aoc, INT, &answer);

    return 0;
}
