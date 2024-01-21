/* Day 9, part 2 = 1071000 */

#include "../../aoc.h"
#include "map.h"

#define VISITED -1

int compare(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}

int basinSize(struct MapData *data, int y, int x) {
    int size = 0;

    if (data->map[y][x] < 9 && data->map[y][x] != VISITED) {
        ++size;
        data->map[y][x] = VISITED;

        if (x > 0) {
            size += basinSize(data, y, x - 1);
        }

        if (x < data->width - 1) {
            size += basinSize(data, y, x + 1);
        }

        if (y > 0) {
            size += basinSize(data, y - 1, x);
        }

        if (y < data->height - 1) {
            size += basinSize(data, y + 1, x);
        }
    }

    return size;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct MapData *data = getMapData(aoc.input);
    int answer = 0;

    if (data) {
        int basinSizes[3] = { 0, 0, 0 };

        for (int y = 0; y < data->height; y++) {
            for (int x = 0; x < data->width; x++) {
                if (isLowPoint(data, y, x)) {
                    int size = basinSize(data, y, x);

                    for (int i = 0; i < 3; i++) {
                        if (size > basinSizes[i]) {
                            basinSizes[i] = size;

                            qsort(basinSizes, 3, sizeof(int), compare);

                            break;
                        }
                    }
                }
            }
        }

        freeMapData(data);

        answer = basinSizes[0] * basinSizes[1] * basinSizes[2];
    }

    print(aoc, INT, &answer);

    return 0;
}
