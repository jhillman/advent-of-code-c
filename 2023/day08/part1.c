/* Day 8, part 1 = 16271 */

#include "../../aoc.h"
#include "map.h"

bool test(struct Node *node) {
    return strcmp(node->name, "ZZZ") == 0;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Map *map = getMap(aoc.input);

    if (map) {
        long answer = getSteps(map, map->nodes->data, test);

        freeMap(map);

        print(aoc, LONG, &answer);
    }

    return 0;
}
