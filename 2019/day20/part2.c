/* Day 20, part 2 = 7248 */

#include "../../aoc.h"
#include "portals.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Maze *maze = getMaze(aoc.input);
    int answer = 0;

    if (maze) {
        answer = fewestSteps(maze, true);

        freeMaze(maze);
    }

    print(aoc, INT, &answer);

    return 0;
}
