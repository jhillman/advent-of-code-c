/* Day 5, part 1 = PTWLTDSJV */

#include "../../aoc.h"
#include "crates.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    char topCrates[16] = { 0 };

    moveCrates(aoc.input, V9000, topCrates);

    print(aoc, STRING, topCrates);

    return 0;
}
