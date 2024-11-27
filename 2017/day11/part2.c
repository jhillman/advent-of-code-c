/* Day 11, part 2 = 1406 */

#include "../../aoc.h"
#include "hex.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = steps(aoc.input, MAX);

    print(aoc, INT, &answer);

    return 0;
}
