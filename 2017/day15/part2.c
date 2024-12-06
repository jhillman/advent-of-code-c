/* Day 15, part 2 = 323 */

#include "../../aoc.h"
#include "generators.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = matchingPairs(aoc.input, 0b011, 0b111, 5000000);

    print(aoc, INT, &answer);

    return 0;
}
