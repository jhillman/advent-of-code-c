#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

struct Cave {
    char **space;
    int left;
    int right;
    int bottom;
    bool hasFloor;
};

bool canMoveDown(struct Cave *cave, int x, int y) {
    return y < cave->bottom && cave->space[y + 1][x - cave->left] == '.';
}

bool canMoveLeft(struct Cave *cave, int x, int y) {
    if (y < cave->bottom - 1) {
        if (x > cave->left) {
            return cave->space[y + 1][(x - cave->left) - 1] == '.';
        } else if (cave->hasFloor && x == cave->left) {
            --cave->left;

            for (int y = 0; y <= cave->bottom; y++) {
                cave->space[y] = (char *)realloc(cave->space[y], ((cave->right - cave->left) + 2) * sizeof(char));
                memcpy(cave->space[y] + 1, cave->space[y], (cave->right - cave->left) * sizeof(char));

                *cave->space[y] = y < cave->bottom ? '.' : '#';
            }

            return true;
        }
    }

    return false;
}

bool canMoveRight(struct Cave *cave, int x, int y) {
    if (y < cave->bottom - 1) {
        if (x < cave->right - 1) {
            return cave->space[y + 1][(x - cave->left) + 1] == '.';
        } else if (cave->hasFloor && x == cave->right - 1) {
            ++cave->right;

            for (int y = 0; y <= cave->bottom; y++) {
                cave->space[y] = (char *)realloc(cave->space[y], ((cave->right - cave->left) + 2) * sizeof(char));

                cave->space[y][(cave->right - cave->left) + 0] = y < cave->bottom ? '.' : '#';
            }

            return true;
        }
    }

    return false;
}

int pourSand(struct Cave *cave) {
    int x;
    int y;
    int grainCount = 0;
    bool canContinue = true;
    bool resting;

    while (canContinue) {
        x = 500;
        y = 0;
        resting = false;

        while (!resting && canContinue) {
            while (canContinue && canMoveDown(cave, x, y)) {
                if (++y == cave->bottom) {
                    canContinue = false;
                }
            }

            if (canContinue) {
                if (canMoveLeft(cave, x, y)) {
                    ++y;

                    if (--x == cave->left && !cave->hasFloor) {
                        canContinue = false;
                    }
                } else if (canMoveRight(cave, x, y)) {
                    ++y;

                    if (++x == cave->right && !cave->hasFloor) {
                        canContinue = false;
                    }
                } else {
                    cave->space[y][x - cave->left] = 'o';
                    resting = true;
                    ++grainCount;

                    if (y == 0) {
                        canContinue = false;
                    }
                }
            }
        }
    }

    return grainCount;
}

struct Cave *getCave(char *input, bool withFloor) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        char line[512];
        char *coordinates;
        int x;
        int y;
        struct Cave *cave = (struct Cave *)calloc(1, sizeof(struct Cave));
        cave->left = 1000;
        cave->hasFloor = withFloor;

        while(fgets(line, sizeof(line), inputFile)) {
            coordinates = strtok(line, " -> ");

            while (coordinates) {
                sscanf(coordinates, "%d,%d", &x, &y);

                cave->left = min(cave->left, x);
                cave->right = max(cave->right, x);
                cave->bottom = max(cave->bottom, y);

                coordinates = strtok(NULL, " -> ");
            }
        }

        if (cave->hasFloor) {
            cave->bottom += 2;
        }

        cave->space = (char **)malloc((cave->bottom + 1) * sizeof(char *));

        for (int y = 0; y <= cave->bottom; y++) {
            cave->space[y] = (char *)malloc(((cave->right - cave->left) + 2) * sizeof(char));
            memset(cave->space[y], '.', (cave->right - cave->left) + 1);
            cave->space[y][(cave->right - cave->left) + 1] = 0;
        }

        fseek(inputFile, 0, SEEK_SET);

        int startX;
        int endX;
        int startY;
        int endY;

        while(fgets(line, sizeof(line), inputFile)) {
            coordinates = strtok(line, " -> ");
            sscanf(coordinates, "%d,%d", &startX, &startY);
            coordinates = strtok(NULL, " -> ");

            do {
                sscanf(coordinates, "%d,%d", &endX, &endY);

                if (startX == endX) {
                    for (int y = min(startY, endY); y <= max(startY, endY); y++) {
                        cave->space[y][endX - cave->left] = '#';
                    }
                } else if (startY == endY) {
                    for (int x = min(startX, endX); x <= max(startX, endX); x++) {
                        cave->space[endY][x - cave->left] = '#';
                    }
                }

                startX = endX;
                startY = endY;

                coordinates = strtok(NULL, " -> ");
            } while (coordinates);
        }

        if (cave->hasFloor) {
            for (int x = cave->left; x <= cave->right; x++) {
                cave->space[cave->bottom][x - cave->left] = '#';
            }
        }

        return cave;
    }

    return NULL;
}

void freeCave(struct Cave *cave) {
    for (int y = 0; y <= cave->bottom; y++) {
        free(cave->space[y]);
    }

    free(cave->space);
    free(cave);
}