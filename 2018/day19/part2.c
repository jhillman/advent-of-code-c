/* Day 19, part 2 = 22674960 */

#include "../../aoc.h"
#include "../device.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Program *program = readProgram(aoc.input);
    int answer = 0;

    if (program) {
        int lastInstructionPointer = -1;
        int maxRegister = 0;

        *program->registers = 1;

        while (program->instructionPointer > lastInstructionPointer) {
            lastInstructionPointer = program->instructionPointer;

            execute(program);
        }

        for (int i = 0; i < 6; i++) {
            if (program->registers[i] > maxRegister) {
                maxRegister = program->registers[i];
            }
        }

        freeProgram(program);

        for (int value = 1; value <= maxRegister; value++) {
            if (maxRegister % value == 0) {
                answer += value;
            }
        }
    }

    print(aoc, INT, &answer);

    return 0;
}
