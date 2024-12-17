/* Day 14, part 2 = 7572 */

#include "../../aoc.h"
#include "space.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    struct Space *space = getSpace(aoc.input);
    int answer = 0;

    if (space) {
        int seconds = 0;
        int line; 
        bool treeDetected = false;

        while (!treeDetected) {
            ++seconds;
            moveRobots(space);

            for (int y = 0; !treeDetected && y < SPACE_HEIGHT; y++) {
                for (int x = 0; !treeDetected && x < SPACE_WIDTH; x++) {
                    if (space->tiles[y][x]) {
                        line = 0;

                        while (x < SPACE_WIDTH && space->tiles[y][x++]) {
                            ++line;
                        }

                        treeDetected = line > 10;
                    }
                }
            }
        }

        answer = seconds;

        freeSpace(space);
    }

    print(aoc, INT, &answer);

    return 0;
}
