/* Day 19, part 2 = 7900946 */

#include "../../aoc.h"
#include "../intcode.h"
#include "drone.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Program *program = loadProgram(aoc.input);
    int answer = 0;

    if (program) {
        struct Drone drone = { X };
        int x = 0;
        int y = 0;

        while (!answer) {
            while (!answer && dronePulled(program, &drone, x + 99, y)) {
                if (dronePulled(program, &drone, x, y + 99) && dronePulled(program, &drone, x + 99, y + 99)) {
                    answer = x * 10000 + y;
                }

                ++x;
            }

            ++y;
        }

        freeProgram(program);
    }

    print(aoc, INT, &answer);

    return 0;
}
