/* Day 16, part 1 = 95476 */

#include "../../aoc.h"
#include "maze.h"

int main(int argc, char **argv) {
struct AoC aoc = init(argc, argv);
    struct Maze *maze = getMaze(aoc.input);
    int answer = 0;

    if (maze) {
        answer = lowestScore(maze);

        freeMaze(maze);
    }

    print(aoc, INT, &answer);

    return 0;
}
