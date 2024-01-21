/* Day 21, part 1 = 194058098264286 */

#include "../../aoc.h"
#include "monkeys.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    int monkeyCount;
    struct Monkey *monkeys = getMonkeys(aoc.input, &monkeyCount, NULL);
    long answer = 0;

    if (monkeys) {
        playGame(monkeys, monkeyCount);

        long rootNumber = *findMonkey("root", monkeys, monkeyCount)->number;

        freeMonkeys(monkeys, monkeyCount);

        answer = rootNumber;
    }

    print(aoc, LONG, &answer);

    return 0;
}
