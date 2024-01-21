/* Day 17, part 1 = 902 */

#include "../../aoc.h"
#include "crucible.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = 0;

    struct HeatLossData *data = getHeatLossData(aoc.input);

    if (data) {
        answer = leastHeatLoss(data, 0, 3);

        freeHeatLossData(data);
    }

    print(aoc, INT, &answer);

    return 0;
}