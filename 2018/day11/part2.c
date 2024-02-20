/* Day 11, part 2 = 235,146,13 */

#include "power.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    char answer[16] = "";
    struct Identifier identifier = findIdentifier(aoc.input, 300);

    sprintf(answer, "%d,%d,%d", identifier.x, identifier.y, identifier.d);

    print(aoc, STRING, answer);

    return 0;
}

