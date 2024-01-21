/* Day 14, part 2 = 100876 */

#include "../../aoc.h"
#include "reflector.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Reflector *reflector = getReflector(aoc.input);
    int answer = 0;

    if (reflector) {
        for (int i = 0; i < 1000; i++) {
            tilt(reflector, NORTH, true);
            tilt(reflector, WEST, false);
            tilt(reflector, SOUTH, true);
            tilt(reflector, EAST, false);
        }

        answer = totalLoad(reflector);

        freeReflector(reflector);
    }

    print(aoc, INT, &answer);
}
