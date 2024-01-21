/* Day 18, part 2 = 2066 */

#include "../../aoc.h"
#include "cubes.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = getArea(aoc.input, true);

    print(aoc, INT, &answer);

    return 0;
}

