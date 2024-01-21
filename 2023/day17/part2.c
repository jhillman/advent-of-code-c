/* Day 17, part 2 = 1073 */

#include "../../aoc.h"
#include "crucible.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct HeatLossData *data = getHeatLossData(aoc.input);
    int answer = 0;

    if (data) {
        answer = leastHeatLoss(data, 4, 10);

        freeHeatLossData(data);
    }

    print(aoc, INT, &answer);

    return 0;
}