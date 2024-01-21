/* Day 12, part 1 = 394 */

#include "../../aoc.h"
#include "elevation.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct ElevationData *data = getElevationData(aoc.input);
    int answer = 0;

    if (data) {
        int steps = fewestSteps(data, data->start, &data->end, 1, true);

        freeElevationData(data);

        answer = steps;
    }

    print(aoc, INT, &answer);

    return 0;
}
