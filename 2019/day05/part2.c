/* Day 5, part 2 = 9217546 */

#include "../../aoc.h"
#include "../intcode.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Program *program = loadProgram(aoc.input);
    int answer = 0;

    if (program) {
        pipe(program->input);
        pipe(program->output);

        int input = 5;
        write(program->input[WRITE], &input, sizeof(aoc.input));

        runProgram(program);

        int output;
        while (read(program->output[READ], &output, sizeof(output)) && output == 0);

        freeProgram(program);

        answer = output;
    }

    print(aoc, INT, &answer);

    return 0;
}
