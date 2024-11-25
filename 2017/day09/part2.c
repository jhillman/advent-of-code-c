/* Day 9, part 2 = 6425 */

#include "../../aoc.h"
#include "stream.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = process(aoc.input, GARBAGE);

    print(aoc, INT, &answer);

    return 0;
}
