/* Day 6, part 2 = 8038 */

#include "../../aoc.h"
#include "memory.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = reallocate(aoc.input, 2);

    print(aoc, INT, &answer);

    return 0;
}
