/* Day 5, part 2 = WZMFVGGZP */

#include "../../aoc.h"
#include "crates.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    char topCrates[16] = { 0 };

    moveCrates(aoc.input, V9001, topCrates);

    print(aoc, STRING, topCrates);

    return 0;
}
