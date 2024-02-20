#include "../../aoc.h"
#include <stdlib.h>
#include <stdbool.h>

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

void fill(char **ground, int width, int height, int x, int y) {
    if (ground[y][x] != '.') {
        return;
    }

    ground[y][x] = '|';
    
    if (y == height - 1) {
        return ;
    }
    
    fill(ground, width, height, x, y + 1);

    if (ground[y + 1][x] == '#' || ground[y + 1][x] == '~') {
        if (x > 0) {
            fill(ground, width, height, x - 1, y);
        }

        if (x < width - 1) {
            fill(ground, width, height, x + 1, y);
        }
    }

    bool bottom = true;

    for (int dx = x; bottom && dx >= 0 && ground[y][dx] != '#'; dx--) {
        bottom = !(ground[y + 1][dx] == '|' || ground[y][dx] == '.');
    }

    for (int dx = x; bottom && dx < width && ground[y][dx] != '#'; dx++) {
        bottom = !(ground[y + 1][dx] == '|' || ground[y][dx] == '.');
    }

    if (bottom) {
        for (int dx = x; dx >= 0 && ground[y][dx] == '|'; dx--) {
            ground[y][dx] = '~';
        }

        for (int dx = x; dx < width && ground[y][dx] == '|'; dx++) {
            ground[y][dx] = '~';
        }
    }
}

int waterQuantity(char *input, char *water) {
    FILE *inputFile = fopen(input, "r");
    int quantity = 0;

    if (inputFile) {
        int x;
        int yStart, yStop;
        int y;
        int xStart, xStop;
        int xMin = 0;
        int xMax = 0;
        int yMin = 0;
        int yMax = 0;
        int width;
        int height;

        while (!feof(inputFile)) {
            switch (fgetc(inputFile)) {
            case 'x':
                fscanf(inputFile, "=%d, y=%d..%d\n", &x, &yStart, &yStop);
                xMin = xMin ? min(xMin, x) : x;
                xMax = xMax ? max(xMax, x) : x;
                yMin = yMin ? min(yMin, yStart) : yStart;
                yMax = yMax ? max(yMax, yStop) : yStop;
                break;
            case 'y':
                fscanf(inputFile, "=%d, x=%d..%d\n", &y, &xStart, &xStop);
                yMin = yMin ? min(yMin, y) : y;
                yMax = yMax ? max(yMax, y) : y;
                xMin = xMin ? min(xMin, xStart) : xStart;
                xMax = xMax ? max(xMax, xStop) : xStop;
                break;
            }
        }

        --xMin;
        width = (xMax - xMin) + 2;
        height = (yMax - yMin) + 1;

        char **ground = (char **)malloc(height * sizeof(char *));

        for (int y = 0; y < height; y++) {
            ground[y] = (char *)malloc(width * sizeof(char));

            memset(ground[y], '.', width * sizeof(char));
        }

        fseek(inputFile, 0, SEEK_SET);

        while (!feof(inputFile)) {
            switch (fgetc(inputFile)) {
            case 'x':
                fscanf(inputFile, "=%d, y=%d..%d\n", &x, &yStart, &yStop);

                for (int y = yStart - yMin; y <= yStop - yMin; y++) {
                    ground[y][x - xMin] = '#';
                }
                break;
            case 'y':
                fscanf(inputFile, "=%d, x=%d..%d\n", &y, &xStart, &xStop);

                for (int x = xStart - xMin; x <= xStop - xMin; x++) {
                    ground[y - yMin][x] = '#';
                }
                break;
            }
        }

        fclose(inputFile);

        fill(ground, width, height, 500 - xMin, 0);

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                quantity += strchr(water, ground[y][x]) ? 1 : 0;
            }

            free(ground[y]);
        }

        free(ground);
    }

    return quantity;
}
