/* Day 7, part 1 = airlri */

#include "../../aoc.h"
#include "programs.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    struct Programs *programs = getPrograms(aoc.input);
    char answer[8] = "";

    if (programs) {
        strcpy(answer, programs->root->name);

        freePrograms(programs);
    }

    print(aoc, STRING, answer);

    return 0;
}
