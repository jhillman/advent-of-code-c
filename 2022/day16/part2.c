/* Day 16, part 2 = 2666 */

#include "../../aoc.h"
#include "valves.h"

struct ValveSets {
    struct ValveName *first;
    int firstCount;
    struct ValveName *second;
    int secondCount;
};

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct ValveData *data = getValveData(aoc.input);
    int answer = 0;

    if (data) {
        struct ValveSets *valveSets = NULL;
        int setCount = 0;

        bool *added = (bool *)calloc(data->flowingValveCount, sizeof(bool));

        int index = 0;

        while (index < data->flowingValveCount) {
            struct ValveSets split = { 
                (struct ValveName *)malloc(data->flowingValveCount * sizeof(struct ValveName)), 
                0, 
                (struct ValveName *)malloc(data->flowingValveCount * sizeof(struct ValveName)),
                0 
            };

            for (int j = 0; j < data->flowingValveCount; j++) {
                if (added[j]) {
                    split.first[split.firstCount++] = data->flowingValveNames[j];
                } else {
                    split.second[split.secondCount++] = data->flowingValveNames[j];
                }
            }

            if (split.firstCount == data->flowingValveCount / 2) {
                if (valveSets) {
                    valveSets = (struct ValveSets *)realloc(valveSets, (setCount + 1) * sizeof(struct ValveSets));
                } else {
                    valveSets = (struct ValveSets *)malloc(sizeof(struct ValveSets));
                }

                valveSets[setCount++] = split;
            }

            index = 0;

            while (index < data->flowingValveCount) {
                added[index] = !added[index];

                if (added[index]) {
                    break;
                }

                ++index;
            }
        }

        free(added);

        struct ValveName one = { "AA", 'A', 'A' };
        bool flowingValves[26][26] = { false };
        int maxPressure = 0;

        for (int i = 0; i < setCount; i++) {
            int firstPressure = findMaxPressure(one, 26, flowingValves, data->valves, valveSets[i].first, valveSets[i].firstCount, data->distances, 0, 0);
            int secondPressure = findMaxPressure(one, 26, flowingValves, data->valves, valveSets[i].second, valveSets[i].secondCount, data->distances, 0, 0);

            maxPressure = max(maxPressure, firstPressure + secondPressure);
        }

        for (int i = 0; i < setCount; i++) {
            free(valveSets[i].first);
            free(valveSets[i].second);
        }

        free(valveSets);

        freeValveData(data);

        answer = maxPressure;
    }

    print(aoc, INT, &answer);

    return 0;
}
