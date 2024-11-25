/* Day 8, part 1 = 6012 */

#include "../../aoc.h"
#include "registers.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = maxRegister(aoc.input, END);

    print(aoc, INT, &answer);

    return 0;
}
