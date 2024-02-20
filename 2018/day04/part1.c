/* Day 4, part 1 = 74743 */

#include "sleep.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct SleepData *data = getSleepData(aoc.input);
    int answer = 0;

    if (data) {
        answer = data->sleepiestGuardMinute;

        freeSleepData(data);
    }

    print(aoc, INT, &answer);

    return 0;
}
