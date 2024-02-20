/* Day 11, part 1 = 243,38 */

#include "power.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    char answer[16] = "";
    struct Identifier identifier = findIdentifier(aoc.input, 3);

    sprintf(answer, "%d,%d", identifier.x, identifier.y);

    print(aoc, STRING, answer);

    return 0;
}
