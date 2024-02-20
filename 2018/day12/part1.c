/* Day 12, part 1 = 3258 */

#include "pots.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    struct Pots *pots = getPots(aoc.input);
    long answer = 0;

    if (pots) {
        int iterations = 20;

        while (iterations--) {
            next(pots);
        }

        answer = plantSum(pots);

        freePots(pots);
    }

    print(aoc, LONG, &answer);

    return 0;
}
