/* Day 24, part 1 = 19923473 */

#include "../../aoc.h"
#include "bugs.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = calculate(aoc.input, BIODIVERSITY_RATING);

    print(aoc, INT, &answer);

    return 0;
}
