/* Day 3, part 1 = 184576302 */

#include "../../aoc.h"
#include "program.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = process(aoc.input, false);

    print(aoc, INT, &answer);

    return 0;
}
