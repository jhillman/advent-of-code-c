/* Day 14, part 1 = 907302 */

#include "../../aoc.h"
#include "stoichiometry.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Stoichiometry *stoichiometry = getStoichiometry(aoc.input);
    int answer = 0;

    if (stoichiometry) {
        answer = countOre(stoichiometry, stoichiometry->fuelId, 1, NULL);

        freeStoichiometry(stoichiometry);
    }

    print(aoc, INT, &answer);

    return 0;
}
