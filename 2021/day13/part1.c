/* Day 13, part 1 = 759 */

#include "../../aoc.h"
#include "dots.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct DotsData *data = getDotsData(aoc.input);
    int answer = 0;

    if (data) {
        answer = foldDots(data, *data->folds);

        freeDotsData(data);
    }

    print(aoc, INT, &answer);

    return 0;
}
