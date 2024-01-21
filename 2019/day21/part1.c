/* Day 21, part 1 = 19357390 */

#include "../../aoc.h"
#include "drone.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Program *program = loadProgram(aoc.input);
    long answer = 0;

    if (program) {
        // struct SpringScript *script = findScript(program, "ABCD", WALK);

        // if (script) {
        //     printScript(script);
        //     freeScript(script);
        // }

        struct SpringScript *script = (struct SpringScript *)calloc(1, sizeof(struct SpringScript));

        char *instructions[] = { 
            "NOT C J",
            "NOT A T",
            "OR T J",
            "AND D J",
            "WALK",
        };

        for (int i = 0; i < (int)(sizeof(instructions) / sizeof(char *)); i++) {
            addTextInstruction(script, instructions[i]);
        }

        answer = getHullDamage(program, script, false);

        freeScript(script);
        freeProgram(program);
    }

    print(aoc, LONG, &answer);

    return 0;
}
