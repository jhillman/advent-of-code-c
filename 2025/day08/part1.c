/* Day 8, part 1 = 131580 */

#include "../../aoc.h"
#include "circuits.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = createCircuits(aoc.input, false);

    print(aoc, INT, &answer);

    return 0;
}
