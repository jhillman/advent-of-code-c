/* Day 15, part 1 = 1065 */

#include "../../aoc.h"
#include "game.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = playGame(aoc.input, 30000000);

    print(aoc, INT, &answer);

    return 0;
}
