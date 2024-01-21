/* Day 20, part 1 = 2275 */

#include "../../aoc.h"
#include "decrypt.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    long answer = decrypt(aoc.input, 1, 1);

    print(aoc, LONG, &answer);

    return 0;
}
