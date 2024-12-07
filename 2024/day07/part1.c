/* Day 7, part 1 = 14711933466277 */

#include "../../aoc.h"
#include "equation.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    long answer = calibration(aoc.input, false);

    print(aoc, LONG, &answer);

    return 0;
}
