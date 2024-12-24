/* Day 20, part 1 = 1293 */

#include "../../aoc.h"
#include "race.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    struct Maze *maze = getMaze(aoc.input);
    int answer = 0;

    if (maze) {
        int cheatOptions = 0;
        int cheatSteps = 2;
        int xDeltas[] = {  0, 0, -1, 1 };
        int yDeltas[] = { -1, 1,  0, 0 };

        race(maze);

        for (int i = 0; i < maze->track->count; i++) {
            struct Location location = maze->track->data[i];

            for (int j = 0; j < 4; j++) {
                int x = location.x + xDeltas[j];
                int y = location.y + yDeltas[j];

                if (maze->grid[y][x] == TRACK) {
                    continue;
                }

                x += xDeltas[j];
                y += yDeltas[j];

                if (!(x >= 0 && x < maze->width && y >= 0 && y < maze->height)) {
                    continue;
                }

                if (maze->times[y][x] < MAX_TIME && (maze->times[y][x] - location.time - cheatSteps) >= 100) {
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
