/* Day 1, part 1 = 1044 */

#include "../../aoc.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        char firstDigit = '\0';
        char previousDigit = '\0';
        char currentDigit;
        int sum = 0;

        do {
            previousDigit = currentDigit;
            currentDigit = fgetc(inputFile);

            if (!firstDigit) {
                firstDigit = currentDigit;
            }

            if (currentDigit == previousDigit) {
                sum += currentDigit - '0';
            }
        } while (!feof(inputFile));

        if (previousDigit == firstDigit) {
            sum += previousDigit - '0';
        }

        fclose(inputFile);

        answer = sum;
    }

    print(aoc, INT, &answer);

    return 0;
}
