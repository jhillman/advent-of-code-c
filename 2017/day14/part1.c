/* Day 14, part 1 = 8304 */

#include "../../aoc.h"
#include "../knot.h"
#include <stdbool.h>

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        char key[16];
        char keyRow[16];
        char hashedKeyRow[33];
        char *h;
        int value;
        int setBitCount[16] = {
            0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4
        };
        int squares = 0;

        fgets(key, sizeof(key), inputFile);
        fclose(inputFile);

        for (int i = 0; i < 128; i++) {
            sprintf(keyRow, "%s-%d", key, i);
            hash(keyRow, hashedKeyRow);

            h = hashedKeyRow;

            for (int j = 0; j < 32; j++) {
                sscanf(h++, "%1x", &value);

                squares += setBitCount[value];
            }

            answer = squares;
        }
    }

    print(aoc, INT, &answer);

    return 0;
}

