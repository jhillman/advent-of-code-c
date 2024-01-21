/* Day 23, part 1 = 2310 */

#include "../../aoc.h"
#include "map.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Map *map = getMap(aoc.input);
    int answer = 0;

    if (map) {
        answer = longestHike(map, ICY);

        freeMap(map);
    }

    print(aoc, INT, &answer);

    return 0;
}
