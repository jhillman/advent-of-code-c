#include <stdlib.h>
#include <stdbool.h>

enum State {
    EMPTY,
    PAPER,
    TO_BE_REMOVED
};

struct Grid {
    int width;
    int height;
    enum State **data;
};

struct Grid *getGrid(char *input) {
    FILE *inputFile = fopen(input, "r");
    struct Grid *grid = NULL;

    if (inputFile) {
        char c;
        grid = (struct Grid *)calloc(1, sizeof(struct Grid));

        while (!feof(inputFile)) {
            c = fgetc(inputFile);

            if (c == '\n' || feof(inputFile)) {
                ++grid->height;
            }

            if (!grid->height) {
                ++grid->width;
            }
        }

        fseek(inputFile, 0, SEEK_SET);

        grid->data = (enum State **)calloc(grid->height, sizeof(enum State *));

        for (int y = 0; y < grid->height; y++) {
            grid->data[y] = (enum State *)calloc(grid->width, sizeof(enum State));

            for (int x = 0; x < grid->width; x++) {
                grid->data[y][x] = fgetc(inputFile) == '@' ? PAPER : EMPTY;
            }

            fgetc(inputFile); // \n
        }

        fclose(inputFile);
    }

    return grid;
}

void printGrid(struct Grid *grid) {
    for (int y = 0; y < grid->height; y++) {
        for (int x = 0; x < grid->width; x++) {
            switch (grid->data[y][x]) {
            case EMPTY:
                printf(".");
                break;
            case PAPER:
                printf("@");
                break;
            case TO_BE_REMOVED:
                printf("-");
                break;
            }
        }

        printf("\n");
    }

    printf("\n");
}

int removeRolls(struct Grid *grid, bool markOnly) {
    int yDeltas[] = { -1, -1, -1,  0, 0,  1, 1, 1 };
    int xDeltas[] = { -1,  0,  1, -1, 1, -1, 0, 1 };
    int removed = 0;

    for (int y = 0; y < grid->height; y++) {
        for (int x = 0; x < grid->width; x++) {
            if (grid->data[y][x] == EMPTY) {
                continue;
            }

            int adjacentRolls = 0;

            for (int i = 0; adjacentRolls < 4 && i < 8; i++) {
                int dy = y + yDeltas[i];
                int dx = x + xDeltas[i];

                if (!(dx >= 0 && dx < grid->width && dy >= 0 && dy < grid->height)) {
                    continue;
                }

                adjacentRolls += grid->data[dy][dx] != EMPTY ? 1 : 0;
            }

            if (adjacentRolls < 4) {
                grid->data[y][x] = markOnly ? TO_BE_REMOVED : EMPTY;
                ++removed;
            }
        }
    }

    return removed;
}

void freeGrid(struct Grid * grid) {
    for (int y = 0; y < grid->height; y++) {
        free(grid->data[y]);
    }

    free(grid->data);
    free(grid);
}