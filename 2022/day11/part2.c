/* Day 11, part 2 = 15305381442 */

#include "../../aoc.h"
#include "monkeys.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    int monkeyCount;
    struct Monkey *monkeys = getMonkeys(aoc.input, &monkeyCount);
    long answer = 0;

    if (monkeys) {
        long business = monkeyBusiness(monkeys, monkeyCount, 10000, TWO);

        freeMonkeys(monkeys, monkeyCount);

        answer = business;
    }

    print(aoc, LONG, &answer);

    return 0;
}
