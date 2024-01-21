/* Day 20, part 1 = 16192267830719 */

#include "../../aoc.h"
#include "tiles.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct TileData *data = getTileData(aoc.input);
    long answer = 0;

    if (data) {
        long product = 1;

        for (int i = 0; i < data->tileCount; i++) {
            int verticalMatchCount = 0;
            int lastVertcialMatch = -1;

            for (int j = 0; j < data->verticalEdgeMatchCount; j++) {
                if (data->verticalEdgeMatches[j].tile1.id == data->tiles[i].id) {
                    if (data->verticalEdgeMatches[j].tile2.id != lastVertcialMatch) {
                        ++verticalMatchCount;
                    }

                    lastVertcialMatch = data->verticalEdgeMatches[j].tile2.id;
                }
            }

            if (verticalMatchCount == 2) {
                product *= data->tiles[i].id;
            }
        }

        freeTileData(data);

        answer = product;
    }

    print(aoc, LONG, &answer);

    return 0;
}
