#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

#define GRID_SIZE 10

struct OctopusData {
    int **octopuses;
    bool **flashed;
};

void printOctopusData(struct OctopusData *data) {
    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            printf("%d", data->octopuses[y][x]);
        }

        printf("\n");
    }

    printf("\n");
}

void flash(struct OctopusData *data, int y, int x) {
    data->flashed[y][x] = true;

    for (int adjacentY = max(0, y - 1); adjacentY <= min(GRID_SIZE - 1, y + 1); adjacentY++) {
        for (int adjacentX = max(0, x - 1); adjacentX <= min(GRID_SIZE - 1, x + 1); adjacentX++) {
            if (!(adjacentX == x && adjacentY == y)) {
                if (++data->octopuses[adjacentY][adjacentX] > 9 && !data->flashed[adjacentY][adjacentX]) {
                    flash(data, adjacentY, adjacentX);
                }
            }
        }
    }
}

int step(struct OctopusData *data) {
    int flashes = 0;

    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            data->flashed[y][x] = false;
            ++data->octopuses[y][x];
        }
    }

    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            if (data->octopuses[y][x] > 9 && !data->flashed[y][x]) {
                flash(data, y, x);
            }
        }
    }

    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            if (data->flashed[y][x]) {
                data->octopuses[y][x] = 0;
                ++flashes;
            }
        }
    }

    return flashes;
}

struct OctopusData *getOctopusData(char *input) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        struct OctopusData *data = (struct OctopusData *)calloc(1, sizeof(struct OctopusData));

        data->octopuses = (int **)calloc(GRID_SIZE, sizeof(int *));
        data->flashed = (bool **)calloc(GRID_SIZE, sizeof(bool *));

        for (int y = 0; y < GRID_SIZE; y++) {
            data->octopuses[y] = (int *)calloc(GRID_SIZE, sizeof(int));
            data->flashed[y] = (bool *)calloc(GRID_SIZE, sizeof(bool));

            for (int x = 0; x < GRID_SIZE; x++) {
                data->octopuses[y][x] = fgetc(inputFile) - '0';
            }

            fgetc(inputFile);
        }

        fclose(inputFile);

        return data;
    }

    return NULL;
}

void freeOctopusData(struct OctopusData *data) {
    for (int y = 0; y < GRID_SIZE; y++) {
        free(data->octopuses[y]);
        free(data->flashed[y]);
    }

    free(data->octopuses);
    free(data->flashed);

    free(data);
}