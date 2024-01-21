/* Day 5, part 1 = 227653707 */

#include "../../aoc.h"
#include "garden.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Garden *garden = getGarden(aoc.input, false);

    if (garden) {
        long answer = 0;

        for (int i = 0; i < garden->seeds->count; i++) {
            long value = processValue(garden, garden->seeds->data[i]);

            if (!answer || value < answer) {
                answer = value;
            }
        }

        freeGarden(garden);

        print(aoc, LONG, &answer);
    }

    return 0;
}
