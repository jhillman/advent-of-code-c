/* Day 12, part 2 = 148962 */

#include "../../aoc.h"
#include "caves.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct CaveData *data = getCaveData(aoc.input);
    int answer = 0;

    if (data) {
        answer = pathCount(data, data->start, 0, true);

        freeCaveData(data);
    }

    print(aoc, INT, &answer);

    return 0;
}
