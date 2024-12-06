/* Day 3, part 2 = 118173507 */

#include "../../aoc.h"
#include "program.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = process(aoc.input, true);

    print(aoc, INT, &answer);

    return 0;
}
