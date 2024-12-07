/* Day 7, part 2 = 286580387663654 */

#include "../../aoc.h"
#include "equation.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    long answer = calibration(aoc.input, true);

    print(aoc, LONG, &answer);

    return 0;
}
