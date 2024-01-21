/* Day 2, part 2 = 1604592846 */

#include "../../aoc.h"
#include <string.h>

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        int position = 0;
        int depth = 0;
        int aim = 0;

        char command[8];
        int units;

        while (!feof(inputFile) && fscanf(inputFile, "%s %d", command, &units)) {
            if (strcmp(command, "forward") == 0) {
                position += units;

                depth += aim * units;
            } else if (strcmp(command, "up") == 0) {
                aim -= units;
            } else if (strcmp(command, "down") == 0) {
                aim += units;
            }
        }

        fclose(inputFile);

        answer = position * depth;
    }

    print(aoc, INT, &answer);

    return 0;
}
