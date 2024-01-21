/* Day 23, part 2 = 52358 */

#include "../../aoc.h"
#define ROOM_DEPTH 4

#include "amphipods.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Burrow burrow;
    int answer = 0;

    if (readBurrow("input2.txt", &burrow)) {
        answer = organizeBurrow(burrow);
    }

    print(aoc, INT, &answer);

    return 0;
}
