/* Day 6, part 2 = 24655068 */

#include "../../aoc.h"
#include "race.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    long answer = getRaceAnswer(aoc.input, true);

    print(aoc, LONG, &answer);
}
