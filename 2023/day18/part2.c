/* Day 18, part 2 = 173152345887206 */

#include "../../aoc.h"
#include "lagoon.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    long answer = lagoonVolume(aoc.input, true);

    print(aoc, LONG, &answer);

    return 0;
}
