/* Day 4, part 2 = 1969 */

#include "../../aoc.h"
#include "puzzle.h"

int xmasCount(struct Puzzle *puzzle, int y, int x) {
    struct Delta deltas[] = {
        { -1, -1 },
        { -1,  1 },
        {  1, -1 },
        {  1,  1 }
    };
    enum Letter letters[][4] = {
        { M, S,
          M, S },
        { S, M,
          S, M },
        { M, M,
          S, S },
        { S, S,
          M, M }
    };
    int count = 0;

    if (xmasPresent(puzzle, y, x, A, A, NULL)) {
        for (int i = 0; i < 4; i++) {
            bool valid = true;
    
            for (int j = 0; valid && j < 4; j++) {
                valid = xmasPresent(puzzle, y + deltas[j].y, x + deltas[j].x, letters[i][j], letters[i][j], NULL);
            }

            if (valid) {
                ++count;
            }
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
