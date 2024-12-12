#include <stdlib.h>
#include <stdbool.h>

struct Map {
    int width;
    int height;
    int **grid;
    bool **visited;
};

void clearVisited(struct Map *map) {
    for (int y = 0; y < map->height; y++) {
        memset(map->visited[y], false, map->width * sizeof(bool));
    }
}

void freeMap(struct Map *map, bool unique) {
    for (int y = 0; y < map->height; y++) {
        free(map->grid[y]);

        if (unique) {
            free(map->visited[y]);
        }
    }

    free(map->grid);
    
    if (unique) {
        free(map->visited);
    }

    free(map);
}

int hike(struct Map *map, int elevation, int x, int y, bool unique) {
    int xDeltas[] = { 0, -1, 1, 0 };
    int yDeltas[] = { -1, 0, 0, 1 };
    int dX;
    int dY;    
    int score = 0;

    if (unique) {
        map->visited[y][x] = true;
    }

    if (map->grid[y][x] == 9) {
        return 1;
    }

    for (int i = 0; i < 4; i++) {
        dX = x + xDeltas[i];
        dY = y + yDeltas[i];

        if (dX < 0 || dX >= map->width || dY < 0 || dY >= map->height) {
            continue;
        }

        if (unique && map->visited[dY][dX]) {
            continue;
        }

        if (map->grid[dY][dX] == elevation) {
            score += hike(map, elevation + 1, dX, dY, unique);
        }
    }

    return score;
}

int score(char *input, bool unique) {
    FILE *inputFile = fopen(input, "r");
    int sum = 0;

    if (inputFile) {
        struct Map *map = (struct Map *)calloc(1, sizeof(struct Map));
        char c;
        int scores = 0;

        while (!feof(inputFile)) {
            c = fgetc(inputFile);

            if (c == '\n' || feof(inputFile)) {
                ++map->height;
            }

            if (!map->height) {
                ++map->width;
            }
        }

        fseek(inputFile, 0, SEEK_SET);

        map->grid = (int **)malloc(map->height * sizeof(int *));

        if (unique) {
            map->visited = (bool **)malloc(map->height * sizeof(bool *));
        }

        for (int y = 0; y < map->height; y++) {
            map->grid[y] = (int *)calloc(map->width, sizeof(int));
        
            if (unique) {
                map->visited[y] = (bool *)calloc(map->width, sizeof(bool));
            }

            for (int x = 0; x < map->width; x++) {
                map->grid[y][x] = fgetc(inputFile) - '0';
            }

            fgetc(inputFile); // \n
        }

        fclose(inputFile);

        for (int y = 0; y < map->height; y++) {
            for (int x = 0; x < map->width; x++) {
                if (map->grid[y][x] == 0) {
                    sum += hike(map, 1, x, y, unique);

                    if (unique) {
                       clearVisited(map);
                    }
                }
            }
        }

        freeMap(map, unique);
    }

    return sum;
}