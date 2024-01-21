/* Day 15, part 1 = 487 */

#include "../../aoc.h"
#include "chitons.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct ChitonsData *data = getChitonsData(aoc.input, 1);
    int answer = 0;

    if (data) {
        answer = lowestRisk(data);

        freeChitonsData(data);
    }

    print(aoc, INT, &answer);

    return 0;
}
