/* Day 20, part 2 = 4090409331120 */

#include "../../aoc.h"
#include "decrypt.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    long answer = decrypt(aoc.input, 811589153, 10);

    print(aoc, LONG, &answer);

    return 0;
}
