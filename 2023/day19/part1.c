/* Day 19, part 1 = 399284 */

#include "../../aoc.h"
#include "parts.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct SortingData *data = getSortingData(aoc.input);
    long answer = 0;

    if (data) {
        for (int i = 0; i < data->parts->count; i++) {
            answer += evaluatePart(data->workflows, "in", data->parts->data[i]);
        }

        freeSortingData(data);
    }

    print(aoc, LONG, &answer);

    return 0;
}
