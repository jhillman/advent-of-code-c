/* Day 12, part 1 = 7928 */

#include "../../aoc.h"
#include "moons.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    int moonCount = 0;
    struct Moon *moons = getMoons(aoc.input, &moonCount);
    int answer = 0;

    if (moons) {
        for (int step = 0; step < 1000; step++) {
            stepMoons(moons, moonCount);
        }

        int totalEnergy = 0;

        for (int i = 0; i < moonCount; i++) {
            totalEnergy += moonEnergy(moons[i]);
        }

        free(moons);

        answer = totalEnergy;
    }

    print(aoc, INT, &answer);

    return 0;
}
