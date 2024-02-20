/* Day 4, part 2 = 132484 */

#include "sleep.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct SleepData *data = getSleepData(aoc.input);
    int answer = 0;

    if (data) {
        answer = data->consistentGuardMinute;

        freeSleepData(data);
    }

    print(aoc, INT, &answer);

    return 0;
}
