/* Day 22, part 2 = 1225064738333321 */

#include "../../aoc.h"
#include "reactor.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    long answer = rebootReactor(aoc.input, false);

    print(aoc, LONG, &answer);

    return 0;
}
