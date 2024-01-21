/* Day 23, part 1 = 15412 */

#include "../../aoc.h"
#define ROOM_DEPTH 2

#include "amphipods.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Burrow burrow;
    long answer = 0;

    if (readBurrow("input1.txt", &burrow)) {
        answer = organizeBurrow(burrow);
    }

    print(aoc, LONG, &answer);

    return 0;
}
