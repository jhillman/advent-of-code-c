/* Day 24, part 2 = 861 */

#include "../../aoc.h"
#include "valley.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct ValleyData *data = getValleyData(aoc.input);
    int answer = 0;

    if (data) {
        int firstTrip = fewestMinutes(data, 0, data->start, data->end);
        int secondTrip = fewestMinutes(data, firstTrip, data->end, data->start);
        int thirdTrip = fewestMinutes(data, firstTrip + secondTrip, data->start, data->end);

        freeValleyData(data);

        answer = firstTrip + secondTrip + thirdTrip;
    }

    print(aoc, INT, &answer);

    return 0;
}
