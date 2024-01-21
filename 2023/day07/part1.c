/* Day 7, part 1 = 249483956 */

#include "../../aoc.h"
#include "cards.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = getWinnings(aoc.input, false);

    print(aoc, INT, &answer);

    return 0;
}
