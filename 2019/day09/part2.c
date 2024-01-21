/* Day 9, part 2 = 87221 */

#include "../../aoc.h"
#include "../intcode.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Program *program = loadProgram(aoc.input);
    long answer = 0;

    if (program) {
        pipe(program->input);
        pipe(program->output);

        int input = 2;
        write(program->input[WRITE], &input, sizeof(aoc.input));

        runProgram(program);

        long output;
        while (read(program->output[READ], &output, sizeof(output)) && output == 0);

        freeProgram(program);

        answer = output;
    }

    print(aoc, LONG, &answer);

    return 0;
}
