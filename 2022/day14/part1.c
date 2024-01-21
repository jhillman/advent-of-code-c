/* Day 14, part 1 = 1068 */

#include "../../aoc.h"
#include "cave.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Cave *cave = getCave(aoc.input, false);
    int answer = 0;

    if (cave) {
        int grainCount = pourSand(cave);

        freeCave(cave);

        answer = grainCount;
    }

    print(aoc, INT, &answer);

    return 0;
}
