#include <stdlib.h>
#include <stdbool.h>

struct Manifold {
    int width;
    int height;
    char **grid;
    long **visited;
};

void printManifold(struct Manifold *manifold) {
    for (int y = 0; y < manifold->height; y++) {
        for (int x = 0; x < manifold->width; x++) {
            printf("%c", manifold->grid[y][x]);
        }

        printf("\n");
    }
}

void freeManifold(struct Manifold *manifold, bool quantum) {
    for (int y = 0; y < manifold->height; y++) {
        free(manifold->grid[y]);

        if (quantum) {
            free(manifold->visited[y]);
        }
    }

    free(manifold->grid);
    
    if (quantum) {
        free(manifold->visited);
    }

    free(manifold);
}

long getTimes(char *input, bool quantum) {
    FILE *inputFile = fopen(input, "r");
    long times = 0;

    if (inputFile) {
        struct Manifold *manifold = (struct Manifold *)calloc(1, sizeof(struct Manifold));
        char c;
        int splits = 0;

        while (!feof(inputFile)) {
            c = fgetc(inputFile);

            if (c == '\n' || feof(inputFile)) {
                ++manifold->height;
            }

            if (!manifold->height) {
                ++manifold->width;
            }
        }

        fseek(inputFile, 0, SEEK_SET);

        manifold->grid = (char **)malloc(manifold->height * sizeof(char *));

        if (quantum) {
            manifold->visited = (long **)malloc(manifold->height * sizeof(long *));
        }

        for (int y = 0; y < manifold->height; y++) {
            manifold->grid[y] = (char *)calloc(manifold->width, sizeof(char));
        
            if (quantum) {
                manifold->visited[y] = (long *)calloc(manifold->width, sizeof(long));
            }

            for (int x = 0; x < manifold->width; x++) {
                manifold->grid[y][x] = fgetc(inputFile);

                if (quantum && y == 0 && manifold->grid[y][x] == 'S') {
                    manifold->visited[y][x] = 1;
                }
            }

            fgetc(inputFile); // \n
        }

        fclose(inputFile);

        for (int y = 1; y < manifold->height; y++) {
            for (int x = 0; x < manifold->width; x++) {
                if (quantum) {
                    long visited = manifold->visited[y - 1][x];

                    if (visited > 0) {
                        if (manifold->grid[y][x] == '^') {
                            manifold->visited[y][x - 1] += visited;
                            manifold->visited[y][x + 1] += visited;
                        } else {
                            manifold->visited[y][x] += visited;
                        }
                    }
                } else {
                    switch (manifold->grid[y - 1][x]) {
                    case 'S':
                        manifold->grid[y][x] = '|';
                        break;
                    case '|':
                        if (manifold->grid[y][x] == '^') {
                            manifold->grid[y][x - 1] = '|';
                            manifold->grid[y][x + 1] = '|';

                            ++splits;
                        } else {
                            manifold->grid[y][x] = '|';
                        }
                    }
                }
            }
        }

        if (quantum) {
            for (int x = 0; x < manifold->width; x++) {
                times += manifold->visited[manifold->height - 1][x];
            }
        } else {
            times = splits;
        }

        freeManifold(manifold, quantum);
    }    

    return times;
}
