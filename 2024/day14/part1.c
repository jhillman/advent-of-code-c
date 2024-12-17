/* Day 14, part 1 = 221616000 */

#include "../../aoc.h"
#include "space.h"

int getQuadrant(int y, int x) {
    int quandrantWidth = (SPACE_WIDTH - SPACE_WIDTH % 2) / 2;
    int quandrantHeight = (SPACE_HEIGHT - SPACE_HEIGHT % 2) / 2;

    if (x == quandrantWidth || y == quandrantHeight) {
        return -1;
    } 

    return y < quandrantHeight ? 
        (x < quandrantWidth ? 0 : 1) : 
        (x < quandrantWidth ? 2 : 3);
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    struct Space *space = getSpace(aoc.input);
    int answer = 0;

    if (space) {
        int quadrants[4] = { 0, 0, 0, 0 };
        int quadrant;

        for (int s = 0; s < 100; s++) {
            moveRobots(space);
        }

        for (int y = 0; y < SPACE_HEIGHT; y++) {
            for (int x = 0; x < SPACE_WIDTH; x++) {
                quadrant = getQuadrant(y, x);

                if (quadrant >= 0) {
                    quadrants[quadrant] += space->tiles[y][x];
                }
            }
        }

        answer = quadrants[0] * quadrants[1] * quadrants[2] * quadrants[3];

        freeSpace(space);
    }

    print(aoc, INT, &answer);

    return 0;
}
