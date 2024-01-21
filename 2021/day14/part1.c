/* Day 14, part 1 = 2657 */

#include "../../aoc.h"
#include "polymer.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct PolymerData *data = getPolymerData(aoc.input);
    long answer = 0;

    if (data) {
        answer = performSteps(data, 10);

        freePolymerData(data);
    }

    print(aoc, LONG, &answer);

    return 0;
}
