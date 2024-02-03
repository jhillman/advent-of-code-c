/* Day 24, part 1 = 254 */

#include "../../aoc.h"
#include "tiles.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct TileData *data = getTileData(aoc.input);
    int answer = 0;

    if (data) {
        answer = countAllBlackTiles(data->center);

        freeTileData(data);
    }

    print(aoc, INT, &answer);

    return 0;
}
