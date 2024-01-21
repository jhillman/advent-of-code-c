/* Day 15, part 1 = 503154 */

#include "../../aoc.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");

    if (inputFile) {
        char c;
        int hash;
        int answer = 0;

        do {
            hash = 0;
            c = fgetc(inputFile);

            while (!feof(inputFile) && c != ',') {
                hash += c;
                hash *= 17;
                hash %= 256;

                c = fgetc(inputFile);
            }

            answer += hash;
        } while (!feof(inputFile));

        fclose(inputFile);

        print(aoc, INT, &answer);
    }

    return 0;
}
