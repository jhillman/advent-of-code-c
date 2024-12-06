/* Day 6, part 1 = 4826 */

#include "../../aoc.h"
#include "map.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    struct Map *map = getMap(aoc.input);
    int answer = 0;

    if (map) {
        patrol(map, NULL);

        answer = map->visited;

        freeMap(map);
    }

    print(aoc, INT, &answer);

    return 0;
}
