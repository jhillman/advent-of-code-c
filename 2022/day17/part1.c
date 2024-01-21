/* Day 17, part 1 = 3206 */

#include "../../aoc.h"
#include "cavern.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Cavern *cavern = getCavern(aoc.input);
    int answer = 0;

    if (cavern) {
        dropRocks(cavern, 2022);

        answer = cavern->top;

        freeCavern(cavern);
    }

    print(aoc, INT, &answer);

    return 0;
}
