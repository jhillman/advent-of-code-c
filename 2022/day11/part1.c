/* Day 11, part 1 = 67830 */

#include "../../aoc.h"
#include "monkeys.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    int monkeyCount;
    struct Monkey *monkeys = getMonkeys(aoc.input, &monkeyCount);
    long answer = 0;

    if (monkeys) {
        int business = monkeyBusiness(monkeys, monkeyCount, 20, ONE);

        freeMonkeys(monkeys, monkeyCount);

        answer = business;
    }

    print(aoc, LONG, &answer);

    return 0;
}
