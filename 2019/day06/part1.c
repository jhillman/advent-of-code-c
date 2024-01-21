/* Day 6, part 1 = 315757 */

#include "../../aoc.h"
#include "orbitmap.h"

int countOrbits(struct Object *object) {
    int count = 0;

    for (int i = 0; i < object->orbiterCount; i++) {
        count += countOrbits(&object->orbiters[i]);
    }

    struct Object *orbitee = object->orbitee;

    while (orbitee) {
        ++count;
        orbitee = orbitee->orbitee;
    }

    return count;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Object *orbitMap = readInput(aoc.input);
    int answer = 0;

    if (orbitMap) {
        answer = countOrbits(orbitMap);

        freeMap(orbitMap);
    }

    print(aoc, INT, &answer);

    return 0;
}
