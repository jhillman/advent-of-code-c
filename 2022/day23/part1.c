/* Day 23, part 1 = 3766 */

#include "../../aoc.h"
#include "grove.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct GroveData *data = getGroveData(aoc.input);
    int answer = 0;

    if (data) {
        for (int i = 0; i < 10; i++) {
            moveElves(data);
        }

        int freeSpace = 0;

        for (int y = data->minElfY; y <= data->maxElfY; y++) {
            for (int x = data->minElfX; x <= data->maxElfX; x++) {
                if (data->grove[y][x] == '.') {
                    ++freeSpace;
                }
            }
        }

        freeGroveData(data);

        answer = freeSpace;
    }

    print(aoc, INT, &answer);

    return 0;
}
