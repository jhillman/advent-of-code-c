/* Day 1, part 2 = 1054 */

#include "../../aoc.h"
#include <stdlib.h>

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        int digitCount = 0;
        char* digits;
        int otherIndex;
        int sum = 0;

        while (fgetc(inputFile) && !feof(inputFile)) {
            ++digitCount;
        }

        fseek(inputFile, 0, SEEK_SET);

        digits = (char *)malloc((digitCount + 1) * sizeof(char));

        fgets(digits, digitCount + 1, inputFile);

        fclose(inputFile);

        for (int index = 0; index < digitCount; index++) {
            otherIndex = (index + digitCount / 2) % digitCount;

            if (digits[index] == digits[otherIndex]) {
                sum += digits[index] - '0';
            }
        }

        free(digits);

        answer = sum;
    }

    print(aoc, INT, &answer);

    return 0;
}
