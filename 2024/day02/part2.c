/* Day 2, part 2 = 349 */

#include "../../aoc.h"
#include "reports.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = safeReportCount(aoc.input, true);

    print(aoc, INT, &answer);

    return 0;
}
