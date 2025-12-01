/* Day 1, part 1 = 1158 */

#include "../../aoc.h"
#include "password.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = password(aoc.input, false);

    print(aoc, INT, &answer);

    return 0;
}
