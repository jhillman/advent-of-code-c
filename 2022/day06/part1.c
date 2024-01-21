/* Day 6, part 1 = 1140 */

#include "../../aoc.h"
#define MARKER_LENGTH 4
#include "marker.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = markerPosition(aoc.input);

    print(aoc, INT, &answer);

    return 0;
}
