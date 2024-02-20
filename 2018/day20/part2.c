/* Day 20, part 2 = 8586 */

#include "regex.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct LocationDistances *locationDistances = getLocationDistances(aoc.input);
    int answer = 0;

    if (locationDistances) {
        answer = locationDistances->distantLocations;

        freeLocationDistances(locationDistances);
    }

    print(aoc, INT, &answer);

    return 0;
}
