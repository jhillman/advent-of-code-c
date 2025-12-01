/* Day 1, part 2 = 6860 */

#include "../../aoc.h"
#include "password.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = password(aoc.input, true);

    print(aoc, INT, &answer);

    return 0;
}