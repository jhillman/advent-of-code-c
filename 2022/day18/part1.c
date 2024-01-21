/* Day 18, part 1 = 3576 */

#include "../../aoc.h"
#include "cubes.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = getArea(aoc.input, false);

    print(aoc, INT, &answer);

    return 0;
}

