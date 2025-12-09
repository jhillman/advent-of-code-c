/* Day 8, part 2 = 6844224 */

#include "../../aoc.h"
#include "circuits.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = createCircuits(aoc.input, true);

    print(aoc, INT, &answer);

    return 0;
}
