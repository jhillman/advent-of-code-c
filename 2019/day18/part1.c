/* Day 18, part 1 = 4246 */

#include "../../aoc.h"
#include "keys.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Map *map = getMap(aoc.input, COMPLETE);
    int answer = 0;

    if (map) {
        answer = stepsToKeys(map, map->entrance, map->keys);

        freeMap(map);
    }

    print(aoc, INT, &answer);

    return 0;
}
