/* Day 10, part 2 = 1609 */

#include "../../aoc.h"
#include "trails.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = score(aoc.input, false);

    print(aoc, INT, &answer);

    return 0;
}
