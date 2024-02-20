/* Day 21, part 1 = 9079325 */

#include "divi.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Program *program = readProgram(aoc.input);
    int answer = 0;

    if (program) {
        loopToIntegerDivision(program);

        int *specialRegister = &program->registers[program->instructions[28].a];

        while (program->instructionPointer < program->length) {
            if (*program->instructionPointerRegister == 28) {
                break;
            }

            execute(program);
        }

        answer = *specialRegister;

        freeProgram(program);
    }

    print(aoc, INT, &answer);

    return 0;
}
