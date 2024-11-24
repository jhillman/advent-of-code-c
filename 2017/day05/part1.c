/* Day 5, part 1 = 356945 */

#include "../../aoc.h"
#include "maze.h"

void updateOffset(int *maze, int instruction) {
    ++maze[instruction];
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = mazeSteps(aoc.input, updateOffset);

    print(aoc, INT, &answer);

    return 0;
}
