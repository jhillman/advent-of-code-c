/* Day 19, part 1 = 2016 */

#include "../../aoc.h"
#include "../device.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Program *program = readProgram(aoc.input);
    int answer = 0;

    if (program) {
        runProgram(program);

        answer = *program->registers;

        freeProgram(program);
    }

    print(aoc, INT, &answer);

    return 0;
}
