/* Day 17, part 2 = 28000 */

#include "reservoir.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = waterQuantity(aoc.input, "~");

    print(aoc, INT, &answer);

    return 0;
}
