/* Day 9, part 2 = 54142584 */

#include "../../aoc.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        int numberCount = 0;
        char ch;

        while (!feof(inputFile)) {
            ch = fgetc(inputFile);

            if (ch == '\n') {
                ++numberCount;
            }
        }

        if (ch != '\n' && numberCount > 0) {
            ++numberCount;
        }

        fseek(inputFile, 0, SEEK_SET);

        long numbers[numberCount];
        int numberIndex = 0;
        long number;

        while (numberIndex < numberCount) {
            fscanf(inputFile, "%ld", &number);

            numbers[numberIndex++] = number;
        }

        fclose(inputFile);

        long invalidNumber = 375054920;
        long weakness = 0;

        for (int i = 0; !weakness && i < numberCount; i++) {
            long sum = numbers[i];
            long largest = numbers[i];
            long smallest = numbers[i];

            for (int j = i + 1; !weakness && sum < invalidNumber && j < numberCount; j++) {
                sum += numbers[j];

                if (numbers[j] > largest) {
                    largest = numbers[j];
                } else if (numbers[j] < smallest) {
                    smallest = numbers[j];
                }

                if (sum == invalidNumber) {
                    weakness = largest + smallest;
                }
            }
        }

       answer = weakness;
    }

    print(aoc, INT, &answer);

    return 0;
}
