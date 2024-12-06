#include <stdlib.h>
#include <stdbool.h>

struct Puzzle {
    int width;
    int height;
    char **grid;
    char letters[5];
};

enum Letter {
    X, M, A, S
};

struct Delta {
    int y;
    int x;
};

bool xmasPresent(struct Puzzle *puzzle, int y, int x, enum Letter letter, enum Letter endLetter, struct Delta *delta) {
    if (y < 0 || y >= puzzle->height || x < 0 || x >= puzzle->width) {
        return false;
    }

    if (puzzle->grid[y][x] == puzzle->letters[letter]) {
        if (letter == endLetter) {
            return true;
        } else {
            return xmasPresent(puzzle, y + delta->y, x + delta->x, letter + 1, endLetter, delta);
        }
    }

    return false;
}

void freePuzzle(struct Puzzle *puzzle) {
    for (int y = 0; y < puzzle->height; y++) {
        free(puzzle->grid[y]);
    }

    free(puzzle->grid);
    free(puzzle);
}

int xmasCount(struct Puzzle *puzzle, int y, int x);

int solvePuzzle(char *input) {
    FILE *inputFile = fopen(input, "r");
    int total = 0;

    if (inputFile) {
        char c;
        struct Puzzle *puzzle = (struct Puzzle *)calloc(1, sizeof(struct Puzzle));
        strcpy(puzzle->letters, "XMAS");

        while (!feof(inputFile)) {
            c = fgetc(inputFile);

            if (c == '\n' || feof(inputFile)) {
                ++puzzle->height;
            }

            if (!puzzle->height) {
                ++puzzle->width;
            }
        }

        fseek(inputFile, 0, SEEK_SET);

        puzzle->grid = (char **)malloc(puzzle->height * sizeof(char *));

        for (int y = 0; y < puzzle->height; y++) {
            puzzle->grid[y] = (char *)malloc(puzzle->width * sizeof(char));

            for (int x = 0; x < puzzle->width; x++) {
                puzzle->grid[y][x] = fgetc(inputFile);
            }

            fgetc(inputFile); // \n
        }

        fclose(inputFile);

        for (int y = 0; y < puzzle->height; y++) {
            for (int x = 0; x < puzzle->width; x++) {
                total += xmasCount(puzzle, y, x);
            }
        }

        freePuzzle(puzzle);
    }

    return total;
}
