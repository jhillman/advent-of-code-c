/* Day 12, part 2 = 388 */

#include "../../aoc.h"
#include "elevation.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct ElevationData *data = getElevationData(aoc.input);
    int answer = 0;

    if (data) {
        struct Location *starts = NULL;
        int startCount = 0;

        for (int y = 0; y < data->height; y++) {
            for (int x = 0; x < data->width; x++) {
                if (data->values[y][x] == 'a') {
                    ++startCount;

                    if (starts) {
                        starts = (struct Location *)realloc(starts, (startCount + 1) * sizeof(struct Location));
                    } else {
                        starts = (struct Location *)malloc((startCount + 1) * sizeof(struct Location));
                    }

                    starts[startCount++] = (struct Location){ x, y, 0 };
                }
            }
        }

        answer = fewestSteps(data, data->end, starts, startCount, false);

        free(starts);
        freeElevationData(data);
    }

    print(aoc, INT, &answer);

    return 0;
}
