/* Day 7, part 2 = 94017638 */

#include "../../aoc.h"
#include "crabs.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct CrabData *data = getCrabData(aoc.input);
    long answer = 0;

    if (data) {
        long minimum = 1000000000;

        for (long position = data->minPosition; position <= data->maxPosition; position++) {
            long fuel = 0;

            for (long i = 0; i < data->crabCount; i++) {
                long distance = labs(data->positions[i] - position);

                fuel += distance > 1 ? distance * (distance + 1) / 2 : distance;
            }

            minimum = min(minimum, fuel);
        }

        freeCrabData(data);

        answer = minimum;
    }

    print(aoc, LONG, &answer);

    return 0;
}
