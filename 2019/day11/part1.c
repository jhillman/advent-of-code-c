/* Day 11, part 1 = 2373 */

#include "../../aoc.h"
#include "robot.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Robot *robot = getRobot(aoc.input);
    int answer = 0;

    if (robot) {
        runRobot(robot, BLACK);

        int painted = 0;

        for (int y = robot->minY; y <= robot->maxY; y++) {
            for (int x = robot->minX; x <= robot->maxX; x++) {
                painted += robot->panels[y][x].painted ? 1 : 0;
            }
        }

        freeRobot(robot);

        answer = painted;
    }

    print(aoc, INT, &answer);

    return 0;
}
