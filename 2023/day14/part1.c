/* Day 14, part 1 = 108935 */

#include "../../aoc.h"
#include "reflector.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Reflector *reflector = getReflector(aoc.input);
    int answer = 0;

    if (reflector) {
        tilt(reflector, NORTH, true);

        answer = totalLoad(reflector);

        freeReflector(reflector);
    }

    print(aoc, INT, &answer);
}
