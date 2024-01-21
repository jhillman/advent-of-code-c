/* Day 9, part 1 = 498 */

#include "../../aoc.h"
#include "map.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct MapData *data = getMapData(aoc.input);
    int answer = 0;

    if (data) {
        for (int y = 0; y < data->height; y++) {
            for (int x = 0; x < data->width; x++) {
                if (isLowPoint(data, y, x)) {
                    answer += data->map[y][x] + 1;
                }
            }
        }

        freeMapData(data);
    }

    print(aoc, INT, &answer);

    return 0;
}
