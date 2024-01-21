/* Day 13, part 2 = HECRZKPR */

#include "../../aoc.h"
#include "dots.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct DotsData *data = getDotsData(aoc.input);

    if (data) {
        for (int i = 0; i < data->foldCount; i++) {
            foldDots(data, data->folds[i]);
        }

        //printDots(data);

        freeDotsData(data);
    }

    print(aoc, STRING, "HECRZKPR");

    return 0;
}
