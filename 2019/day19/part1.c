/* Day 19, part 1 = 226 */

#include "../../aoc.h"
#include "../intcode.h"
#include "drone.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Program *program = loadProgram(aoc.input);
    int answer = 0;

    if (program) {
        struct Drone drone = { X };

        for (int y = 0; y < 50; y++) {
            for (int x = 0; x < 50; x++) {
                answer += dronePulled(program, &drone, x, y) ? 1 : 0;
            }
        }

        freeProgram(program);
    }

    print(aoc, INT, &answer);

    return 0;
}
