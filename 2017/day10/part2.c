/* Day 10, part 2 = dc7e7dee710d4c7201ce42713e6b8359 */

#include "../../aoc.h"
#include "../knot.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    char answer[33] = "";

    if (inputFile) {
        char input[64];

        fgets(input, sizeof(input), inputFile);
        fclose(inputFile);

        hash(input, answer);
    }

    print(aoc, STRING, answer);

    return 0;
}
