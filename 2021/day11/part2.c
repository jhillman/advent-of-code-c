/* Day 11, part 2 = 371 */

#include "../../aoc.h"
#include "octopuses.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct OctopusData *data = getOctopusData(aoc.input);
    int answer = 0;

    if (data) {
        int stepNumber = 0;

        while (!answer) {
            ++stepNumber;

            if (step(data) == GRID_SIZE * GRID_SIZE) {
                answer = stepNumber;
            }
        }

        freeOctopusData(data);
    }

    print(aoc, INT, &answer);

    return 0;
}
