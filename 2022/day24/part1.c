/* Day 24, part 1 = 288 */

#include "../../aoc.h"
#include "valley.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct ValleyData *data = getValleyData(aoc.input);
    int answer = 0;

    if (data) {
        int minutes = fewestMinutes(data, 0, data->start, data->end);

        freeValleyData(data);

        answer = minutes;
    }

    print(aoc, INT, &answer);

    return 0;
}
