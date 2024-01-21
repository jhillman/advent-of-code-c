/* Day 20, part 2 = 19156 */

#include "../../aoc.h"
#include "image.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = enhance(aoc.input, 50);

    print(aoc, INT, &answer);

    return 0;
}