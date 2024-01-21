/* Day 12, part 1 = 4659 */

#include "../../aoc.h"
#include "caves.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct CaveData *data = getCaveData(aoc.input);
    int answer = 0;

    if (data) {
        answer = pathCount(data, data->start, 0, false);

        freeCaveData(data);
    }

    print(aoc, INT, &answer);

    return 0;
}
