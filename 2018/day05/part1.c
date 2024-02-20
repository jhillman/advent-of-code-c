/* Day 5, part 1 = 9202 */

#include "polymer.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Polymer *polymer = getReactedPolymer(aoc.input);
    int answer = 0;

    if (polymer) {
        answer = polymer->length;

        freePolymer(polymer);
    }

    print(aoc, INT, &answer);

    return 0;
}
