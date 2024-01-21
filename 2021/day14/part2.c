/* Day 14, part 2 = 2911561572630 */

#include "../../aoc.h"
#include "polymer.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct PolymerData *data = getPolymerData(aoc.input);
    long answer = 0;

    if (data) {
        answer = performSteps(data, 40);

        freePolymerData(data);
    }

    print(aoc, LONG, &answer);

    return 0;
}
