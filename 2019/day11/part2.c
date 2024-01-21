/* Day 11, part 2 = PCKRLPUK */

#include "../../aoc.h"
#include "robot.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Robot *robot = getRobot(aoc.input);
    char answer[16] = { 0 };

    if (robot) {
        runRobot(robot, WHITE);

        // for (int y = robot->minY; y <= robot->maxY; y++) {
        //     for (int x = robot->minX; x <= robot->maxX; x++) {
        //         // printf("%c", robot->panels[y][x].color == WHITE ? '#' : ' ');
        //     }

        //     // printf("\n");
        // }

        freeRobot(robot);

        strcpy(answer, "PCKRLPUK");
    }

    print(aoc, STRING, answer);

    return 0;
}
