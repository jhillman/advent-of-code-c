/* Day 11, part 1 = 682 */

#include "../../aoc.h"
#include "hex.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = steps(aoc.input, FEWEST);

    print(aoc, INT, &answer);

    return 0;
}
