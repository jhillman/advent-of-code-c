/* Day 7, part 2 = 252137472 */

#include "../../aoc.h"
#include "cards.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = getWinnings(aoc.input, true);

    print(aoc, INT, &answer);

    return 0;
}