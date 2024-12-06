/* Day 2, part 1 = 282 */

#include "../../aoc.h"
#include "reports.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = safeReportCount(aoc.input, false);

    print(aoc, INT, &answer);

    return 0;
}
