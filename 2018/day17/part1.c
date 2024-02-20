/* Day 17, part 1 = 34541 */

#include "reservoir.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = waterQuantity(aoc.input, "|~");

    print(aoc, INT, &answer);

    return 0;
}
