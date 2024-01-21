#include <stdlib.h>
#include <string.h>

char **getSeatLayout(char *input, int *rowCount, int *columnCount) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        *rowCount = 0;
        *columnCount = 0;
        char ch = fgetc(inputFile);

        while (ch != '\n') {
            ++*columnCount;
            ch = fgetc(inputFile);
        }

        fseek(inputFile, 0, SEEK_SET);

        while (!feof(inputFile)) {
            ch = fgetc(inputFile);

            if (ch == '\n') {
                ++*rowCount;
            }
        }

        ++*rowCount;

        fseek(inputFile, 0, SEEK_SET);

        char **seatLayout = (char **)calloc(*rowCount, sizeof(char *));

        int rowIndex = 0;

        while (rowIndex < *rowCount) {
            seatLayout[rowIndex] = (char *)calloc(*columnCount + 1, sizeof(char));
            fscanf(inputFile, "%s", seatLayout[rowIndex++]);
        }

        fclose(inputFile);

        return seatLayout;
    }    

    return NULL;
}

void freeLayout(char **seatLayout, int rowCount) {
    for (int i = 0; i < rowCount; i++) {
        free(seatLayout[i]);
    }

    free(seatLayout);
}

int occupiedSeatCount(char **seatLayout, int rowCount, int columnCount, int row, int column);

int applyRules(char **seatLayout, int rowCount, int columnCount, int minSeatCount) {
    int changes = 0;
    char **seatLayoutCopy = (char **)calloc(rowCount, sizeof(char *));

    for (int row = 0; row < rowCount; row++) {
        seatLayoutCopy[row] = (char *)calloc(columnCount + 1, sizeof(char));
        strcpy(seatLayoutCopy[row], seatLayout[row]);
    }

    for (int row = 0; row < rowCount; row++) {
        for (int column = 0; column < columnCount; column++) {
            switch (seatLayoutCopy[row][column]) {
                case 'L':
                    if (occupiedSeatCount(seatLayoutCopy, rowCount, columnCount, row, column) == 0) {
                        ++changes;
                        seatLayout[row][column] = '#';
                    }
                    break;
                case '#':
                    if (occupiedSeatCount(seatLayoutCopy, rowCount, columnCount, row, column) >= minSeatCount) {
                        ++changes;
                        seatLayout[row][column] = 'L';
                    }
                    break;
            }
        }
    }

    freeLayout(seatLayoutCopy, rowCount);

    return changes;
}

int totalOccupiedSeatCount(char **seatLayout, int rowCount, int columnCount) {
    int count = 0;

    for (int row = 0; row < rowCount; row++) {
        for (int column = 0; column < columnCount; column++) {
            if (seatLayout[row][column] == '#') {
                ++count;
            }
        }
    }

    return count;
}
