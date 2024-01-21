/* Day 1, part 2 = 49214880 */

#include "../../aoc.h"
#include <stdlib.h>
#include <stdbool.h>

int compare(const void *a, const void *b) {
    return *(int *)b - *(int *)a;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        int numberCount = 200;
        int targetSum = 2020;
        int numbers[numberCount];
        int number;

        for (int i = 0; i < numberCount; i++) {
            fscanf(inputFile, "%d", &number);
            numbers[i] = number;
        }

        fclose(inputFile);

        qsort(numbers, numberCount, sizeof(int), compare);

        int addend1;
        int addend2;
        int addend3;
        bool found = false;

        for (int i = 0; !found && i < numberCount; i++) {
            addend1 = numbers[i];

            for (int j = numberCount - 1; !found && j > i; j--) {
                addend2 = numbers[j];

                for (int k = numberCount - 1; !found && k > j; k--) {
                    addend3 = numbers[k];

                    if (addend1 + addend2 + addend3 == targetSum) {
                        found = true;
                    }
                }
            }
        }

        answer = addend1 * addend2 * addend3;
    }

    print(aoc, INT, &answer);

    return 0;
}