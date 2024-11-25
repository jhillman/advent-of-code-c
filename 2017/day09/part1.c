/* Day 9, part 1 = 12803 */

#include "../../aoc.h"
#include "stream.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = process(aoc.input, SCORE);

    print(aoc, INT, &answer);

    return 0;
}
