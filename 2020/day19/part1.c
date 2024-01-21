/* Day 19, part 1 = 187 */

#include "../../aoc.h"
#include "match.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct RulesData *data = readRulesData(aoc.input);
    int answer = 0;

    if (data) {
        int validCount = validInputCount(data);

        freeRulesData(data);

        answer = validCount;
    }

    print(aoc, INT, &answer);

    return 0;
}
