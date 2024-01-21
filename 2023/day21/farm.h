#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct Location {
    int x;
    int y;
};

struct Farm {
    int width;
    int height;
    
    struct Location start;
    int maxSteps;

    char **tiles;
    bool ***visited;
};

struct Farm *getFarm(char *input, int maxSteps) {
    FILE *inputFile = fopen(input, "r");
    struct Farm *farm;

    if (inputFile) {
        char c;
        farm = (struct Farm *)calloc(1, sizeof(struct Farm));
        farm->maxSteps = maxSteps;

        while (!feof(inputFile)) {
            c = fgetc(inputFile);

            if (c == '\n' || feof(inputFile)) {
                ++farm->height;
            }

            if (!farm->height) {
                ++farm->width;
            }
        }

        fseek(inputFile, 0, SEEK_SET);

        farm->tiles = (char **)malloc(farm->height * sizeof(char *));
        farm->visited = (bool ***)malloc(farm->height * sizeof(bool **));

        for (int y = 0; y < farm->height; y++) {
            farm->tiles[y] = (char *)malloc(farm->width * sizeof(char));
            farm->visited[y] = (bool **)malloc(farm->width * sizeof(bool *));

            for (int x = 0; x < farm->width; x++) {
                farm->tiles[y][x] = fgetc(inputFile);
                farm->visited[y][x] = (bool *)calloc(maxSteps + 1, sizeof(bool));

                if (farm->tiles[y][x] == 'S') {
                    farm->start.x = x;
                    farm->start.y = y;
                }
            }

            fgetc(inputFile); // \n
        }

        fclose(inputFile);
    }

    return farm;
}

void clearVisited(struct Farm *farm) {
    for (int y = 0; y < farm->height; y++) {
        for (int x = 0; x < farm->width; x++) {
            memset(farm->visited[y][x], false, sizeof(bool) * farm->maxSteps);
        }
    }
}

long reachablePlots(struct Farm *farm, struct Location location, int maxSteps, long steps) {
    long plots = 0;
    int xDeltas[] = {-1, 0, 1, 0};
    int yDeltas[] = {0, 1, 0, -1};

    farm->visited[location.y][location.x][steps] = true;

    if (steps == maxSteps) {
        plots = 1;
    } else {
        for (int i = 0; i < 4; i++) {
            struct Location nextLocation = { location.x + xDeltas[i], location.y + yDeltas[i] };

            if (!(nextLocation.x >= 0 && nextLocation.x < farm->width && nextLocation.y >= 0 && nextLocation.y < farm->height)) {
                continue;
            }

            if (farm->tiles[nextLocation.y][nextLocation.x] == '#' || farm->visited[nextLocation.y][nextLocation.x][steps + 1]) {
                continue;
            }

            plots += reachablePlots(farm, nextLocation, maxSteps, steps + 1);
        }
    }

    return plots;
}

void freeFarm(struct Farm *farm) {
    for (int y = 0; y < farm->height; y++) {
        for (int x = 0; x < farm->width; x++) {
            free(farm->visited[y][x]);
        }

        free(farm->tiles[y]);
        free(farm->visited[y]);
    }

    free(farm->tiles);
    free(farm->visited);
    free(farm);
}