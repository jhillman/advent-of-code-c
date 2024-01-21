/* Day 11, part 1 = 1620 */

#include "../../aoc.h"
#include "octopuses.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct OctopusData *data = getOctopusData(aoc.input);
    int answer = 0;

    if (data) {
        for (int i = 0; i < 100; i++) {
            answer += step(data);
        }

        freeOctopusData(data);
    }

    print(aoc, INT, &answer);

    return 0;
}
