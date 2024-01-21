/* Day 6, part 1 = 3317888 */

#include "../../aoc.h"
#include "race.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    long answer = getRaceAnswer(aoc.input, false);

    print(aoc, LONG, &answer);
}
