/* Day 2, part 1 = 2842648 */

#include "../../aoc.h"
#include "../intcode.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Program *program = loadProgram(aoc.input);
    long answer = 0;

    if (program) {
        initializeProgram(program, 12, 2);

        runProgram(program);

        answer = *program->data;

        freeProgram(program);
    }

    print(aoc, LONG, &answer);

    return 0;
}