#include <stdio.h>
#include <stdlib.h>

int **getTreeHeights(char *input, int *width, int *height) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        *width = 0;
        *height = 0;

        char c = fgetc(inputFile);

        while (!feof(inputFile)) {
            if (c == '\n') {
                ++*height;
            }

            if (!*height) {
                ++*width;
            }

            c = fgetc(inputFile);
        }

        ++*height;

        fseek(inputFile, 0, SEEK_SET);

        int **treeHeights = (int **)calloc(*height, sizeof(int *));

        for (int y = 0; y < *height; y++) {
            treeHeights[y] = (int *)calloc(*width, sizeof(int));

            for (int x = 0; x < *width; x++) {
                treeHeights[y][x] = fgetc(inputFile) - '0';
            }

            if (y < *height) {
                fgetc(inputFile);
            }
        }

        fclose(inputFile);

        return treeHeights;
    }

    return NULL;
}