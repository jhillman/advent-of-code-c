/* Day 6, part 2 = 1721 */

#include "../../aoc.h"
#include "map.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    struct Map *map = getMap(aoc.input);
    int answer = 0;

    if (map) {
        struct Positions *positions = (struct Positions *)calloc(1, sizeof(struct Positions));
        int loops = 0;

        patrol(map, positions);

        for (int i = 0; i < positions->count; i++) {
            map->grid[positions->data[i].y][positions->data[i].x].type = OBSTRUCTION;

            if (patrol(map, NULL) == LOOP) {
                ++loops;
            }

            map->grid[positions->data[i].y][positions->data[i].x].type = PATH;

            resetMap(map);
        }

        answer = loops;

        freePositions(positions);
        freeMap(map);
    }

    print(aoc, INT, &answer);

    return 0;
}
