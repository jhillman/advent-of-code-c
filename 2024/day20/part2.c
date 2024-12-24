/* Day 20, part 2 = 977747 */

#include "../../aoc.h"
#include "race.h"
#include <math.h>

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    struct Maze *maze = getMaze(aoc.input);
    int answer = 0;

    if (maze) {
        int cheatOptions = 0;
        int xDeltas[] = {  0, 0, -1, 1 };
        int yDeltas[] = { -1, 1,  0, 0 };

        race(maze);

        for (int i = 0; i < maze->track->count; i++) {
            struct Location location = maze->track->data[i];

            for (int j = i + 100; j < maze->track->count; j++) {
                struct Location cheatLocation = maze->track->data[j];
                int cheatSteps = abs(cheatLocation.y - location.y) + abs(cheatLocation.x - location.x);

                if (cheatSteps <= 20 && (maze->times[cheatLocation.y][cheatLocation.x] - location.time - cheatSteps) >= 100) {
                    ++cheatOptions;
                }
            }
        }

        answer = cheatOptions;

        freeMaze(maze);
    }

    print(aoc, INT, &answer);

    return 0;
}
