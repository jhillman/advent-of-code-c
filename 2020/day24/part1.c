/* Day 24, part 1 = 254 */

#include "../../aoc.h"
#include "tiles.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Tile *center = getTiles(aoc.input);
    int answer = 0;

    if (center) {
        answer = countAllBlackTiles(center);

        freeTile(center);
    }

    print(aoc, INT, &answer);

    return 0;
}
