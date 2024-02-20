/* Day 12, part 2 = 3600000002022 */

#include "pots.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    struct Pots *pots = getPots(aoc.input);
    long answer = 0;

    if (pots) {
        long iterations = 50000000000;

        while (iterations--) {
            next(pots);

            if (strcmp(pots->currentGeneration, pots->previousGeneration) == 0) {
                pots->left += iterations * (pots->left - pots->previousLeft);
                break;
            }
        }

        answer = plantSum(pots);

        freePots(pots);
    }

    print(aoc, LONG, &answer);

    return 0;
}
