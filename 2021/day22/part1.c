/* Day 22, part 1 = 590467 */

#include "../../aoc.h"
#include "reactor.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    long answer = rebootReactor(aoc.input, true);

    print(aoc, LONG, &answer);

    return 0;
}
