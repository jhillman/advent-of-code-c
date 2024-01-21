/* Day 18, part 1 = 3816 */

#include "../../aoc.h"
#include "snailfish.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    long answer = 0;

    if (inputFile) {
        char numberString[64];
        struct SnailfishNumber *number = NULL;

        while (fgets(numberString, sizeof(numberString), inputFile)) {
            if (number) {
                number = add(number, readNumber(NULL, numberString, NULL));
            } else {
                number = readNumber(NULL, numberString, NULL);
            }
        }

        fclose(inputFile);

        answer = magnitude(number);

        freeNumber(number);
    }

    print(aoc, LONG, &answer);

    return 0;
}
