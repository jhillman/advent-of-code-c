/* Day 4, part 2 = 8948 */

#include "../../aoc.h"
#include "paper.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    struct Grid *grid = getGrid(aoc.input);
    int answer = 0;

    if (grid) {
        int removed;
        int totalRemoved = 0;

        do {
            totalRemoved += (removed = removeRolls(grid, false));
        } while (removed);

        freeGrid(grid);

        answer = totalRemoved;
    }

    print(aoc, INT, &answer);

    return 0;
}
