/* Day 4, part 1 = 1547 */

#include "../../aoc.h"
#include "paper.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    struct Grid *grid = getGrid(aoc.input);
    int answer = 0;

    if (grid) {
        answer = removeRolls(grid, true);

        freeGrid(grid);
    }

    print(aoc, INT, &answer);

    return 0;
}
