/* Day 23, part 2 = 954 */

#include "../../aoc.h"
#include "grove.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct GroveData *data = getGroveData(aoc.input);
    int answer = 0;

    if (data) {
        int rounds = 0;

        do {
            ++rounds;
        } while (moveElves(data) > 0);

        freeGroveData(data);

        answer = rounds;
    }

    print(aoc, INT, &answer);

    return 0;
}
