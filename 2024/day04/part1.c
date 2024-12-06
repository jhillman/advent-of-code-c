/* Day 4, part 1 = 2507 */

#include "../../aoc.h"
#include "puzzle.h"

int xmasCount(struct Puzzle *puzzle, int y, int x) {
    int count = 0;
    struct Delta deltas[] = {
        { -1,  0 },
        { -1,  1 },
        {  0,  1 },
        {  1,  1 },
        {  1,  0 },
        {  1, -1 },
        {  0, -1 },
        { -1, -1 }
    };

    for (int d = 0; d < 8; d++) {
        if (xmasPresent(puzzle, y, x, X, S, &deltas[d])) {
            ++count;
        }
    }

    return count;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = solvePuzzle(aoc.input);

    print(aoc, INT, &answer);

    return 0;
}
