/* Day 10, part 1 = 754 */

#include "../../aoc.h"
#include "trails.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = score(aoc.input, true);

    print(aoc, INT, &answer);

    return 0;
}
