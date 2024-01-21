/* Day 25, part 1 = 8912902 */

#include "game.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Program *program = loadProgram(aoc.input);
    enum GameMode mode = INSTRUCTIONS;
    int answer = 0;

    if (program) {
        answer = playGame(program, mode);

        freeProgram(program);
    }

    if (mode != MANUAL) {
        print(aoc, INT, &answer);
    }

    return 0;
}
