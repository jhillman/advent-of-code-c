/* Day 7, part 1 = 333755 */

#include "../../aoc.h"
#include "crabs.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct CrabData *data = getCrabData(aoc.input);
    int answer = 0;

    if (data) {
        int minimum = 1000000000;

        for (int position = data->minPosition; position <= data->maxPosition; position++) {
            int fuel = 0;

            for (int i = 0; i < data->crabCount; i++) {
                fuel += labs(data->positions[i] - position);
            }

            minimum = min(minimum, fuel);
        }

        freeCrabData(data);

        answer = minimum;
    }

    print(aoc, INT, &answer);

    return 0;
}
