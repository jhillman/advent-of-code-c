/* Day 21, part 2 = 3715167 */

#include "divi.h"
#include <stdbool.h>

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Program *program = readProgram(aoc.input);
    int answer = 0;

    if (program) {
        loopToIntegerDivision(program);

        int *specialRegister = &program->registers[program->instructions[28].a];
        bool *seen = (bool *)calloc(50000000, sizeof(bool));
        int previous;

        while (program->instructionPointer < program->length) {
            if (*program->instructionPointerRegister == 28) {
                if (seen[*specialRegister]) {
                    break;
                }

                previous = *specialRegister;
                seen[*specialRegister] = true;
            }

            execute(program);
        }

        answer = previous;

        free(seen);
        freeProgram(program);
    }

    print(aoc, INT, &answer);

    return 0;
}
