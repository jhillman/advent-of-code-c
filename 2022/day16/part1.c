/* Day 16, part 1 = 2114 */

#include "../../aoc.h"
#include "valves.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct ValveData *data = getValveData(aoc.input);
    int answer = 0;

    if (data) {
        struct ValveName one = { "AA", 'A', 'A' };
        bool flowingValves[26][26] = { false };
        int maxPressure = findMaxPressure(one, 30, flowingValves, data->valves, data->flowingValveNames, data->flowingValveCount, data->distances, 0, 0);

        freeValveData(data);

        answer = maxPressure;
    }

    print(aoc, INT, &answer);

    return 0;
}
