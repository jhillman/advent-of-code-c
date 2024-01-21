#include <stdio.h>
#include <stdlib.h>

int playGame(char *input, int turnCount) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        int position = 0;
        int number;
        char ch;
        int *positions = (int *) calloc(turnCount, sizeof(int));

        while (fscanf(inputFile, "%d%c", &number, &ch) >= 1) {
            positions[number] = position + 1;

            ++position;
        }

        fclose(inputFile);

        int lastNumber = number;

        while (position < turnCount) {
            if (positions[lastNumber] != 0) {
                number = position - positions[lastNumber];
            } else {
                number = 0;
            }

            positions[lastNumber] = position;
            lastNumber = number;

            ++position;
        }

        free(positions);

        return number;
    }

    return 0;
}
