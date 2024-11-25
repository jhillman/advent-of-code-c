/* Day 8, part 2 = 6369 */

#include "../../aoc.h"
#include "registers.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = maxRegister(aoc.input, OVERALL);

    print(aoc, INT, &answer);

    return 0;
}
