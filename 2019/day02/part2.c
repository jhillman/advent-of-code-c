/* Day 2, part 2 = 9074 */

#include "../../aoc.h"
#include <stdbool.h>
#include "../intcode.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Program *program = loadProgram(aoc.input);
    int answer = 0;

    if (program) {
        int targetOutput = 19690720;
        struct Program *testProgram = NULL;
        bool outputFound = false;

        for (int noun = 0; !outputFound && noun < 100; noun++) {
            for (int verb = 0; !outputFound && verb < 100; verb++) {
                freeProgram(testProgram);

                testProgram = copyProgram(program, 0);

                initializeProgram(testProgram, noun, verb);

                runProgram(testProgram);

                if (*testProgram->data == targetOutput) {
                    outputFound = true;
                }         
            }
        }

        answer = 100 * programNoun(testProgram) + programVerb(testProgram);

        freeProgram(testProgram);
        freeProgram(program);
    }

    print(aoc, INT, &answer);

    return 0;
}