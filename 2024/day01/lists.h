#include <stdlib.h>

void getLists(char * input, int **listOne, int **listTwo, int *numberCount) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        char c;
        *numberCount = 0;

        while (!feof(inputFile) && (c = fgetc(inputFile))) {
            if (feof(inputFile) || c == '\n') {
                ++*numberCount;
            }
        }

        fseek(inputFile, 0, SEEK_SET);

        *listOne = (int *)malloc(*numberCount * sizeof(int));
        *listTwo = (int *)malloc(*numberCount * sizeof(int));

        for (int i = 0; i < *numberCount; i++) {
            fscanf(inputFile, "%d %d", *listOne + i, *listTwo + i);
        }

        fclose(inputFile);
    }
}
