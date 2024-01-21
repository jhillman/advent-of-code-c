/* Day 18, part 2 = 1940 */

#include "../../aoc.h"
#include "keys.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = 0;

    enum MapType types[] = { UPPER_LEFT, UPPER_RIGHT, LOWER_LEFT, LOWER_RIGHT };

    for (int i = 0; i < 4; i++) {
        struct Map *map = getMap(aoc.input, types[i]);

        if (map) {
            answer += stepsToKeys(map, map->entrance, map->keys);

            freeMap(map);
        }
    }

    print(aoc, INT, &answer);

    return 0;
}
