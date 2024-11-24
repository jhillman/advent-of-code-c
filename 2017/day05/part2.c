/* Day 5, part 2 = 28372145 */

#include "../../aoc.h"
#include "maze.h"

void updateOffset(int *maze, int instruction) {
    if (maze[instruction] >= 3) {
        --maze[instruction];
    } else {
        ++maze[instruction];
    }   
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = mazeSteps(aoc.input, updateOffset);

    print(aoc, INT, &answer);

    return 0;
}