/* Day 14, part 2 = 1670299 */

#include "../../aoc.h"
#include "stoichiometry.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Stoichiometry *stoichiometry = getStoichiometry(aoc.input);
    long answer = 0;

    if (stoichiometry) {
        double minFuel = 1;
        double maxFuel = 100000000;
        long oreCount;

        while (maxFuel - minFuel > 1) {
            oreCount = round((minFuel + maxFuel) / 2.0);
         
            if (countOre(stoichiometry, stoichiometry->fuelId, oreCount, NULL) <= 1000000000000) {
                minFuel = oreCount;
            } else {
                maxFuel = oreCount;
            }
        }

        answer = minFuel;

        freeStoichiometry(stoichiometry);
    }

    print(aoc, LONG, &answer);

    return 0;
}
