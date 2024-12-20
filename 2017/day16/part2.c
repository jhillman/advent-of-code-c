/* Day 16, part 2 = fbidepghmjklcnoa */

#include "../../aoc.h"
#include "programs.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    char answer[17];

    char *programs = dance(aoc.input, calloc(1, sizeof(struct ProgramsList)), 1000000000);

    if (programs) {
        strcpy(answer, programs);

        free(programs);
    }

    print(aoc, STRING, &answer);

    return 0;
}