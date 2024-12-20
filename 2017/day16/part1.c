/* Day 16, part 1 = hmefajngplkidocb */

#include "../../aoc.h"
#include "programs.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    char answer[17];

    char *programs = dance(aoc.input, NULL, 1);

    if (programs) {
        strcpy(answer, programs);

        free(programs);
    }

    print(aoc, STRING, &answer);

    return 0;
}
