#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct MapData {
    int width;
    int height;
    int **map;
};

struct MapData *getMapData(char *input) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        struct MapData *data = (struct MapData *)calloc(1, sizeof(struct MapData));

        char character;

        while (fscanf(inputFile, "%c", &character) > 0) {
            if (character == '\n') {
                ++data->height;
            }

            if (data->height == 0) {
                ++data->width;
            }
        }

        ++data->height;

        fseek(inputFile, 0, SEEK_SET);

        data->map = (int **)calloc(data->height, sizeof(int *));

        for (int y = 0; y < data->height; y++) {
            data->map[y] = (int *)calloc(data->width, sizeof(int));

            for (int x = 0; x < data->width; x++) {
                character = fgetc(inputFile);

                data->map[y][x] = character - '0';
            }

            // \n
            fgetc(inputFile);
        }

        fclose(inputFile);


        return data;
    }

    return NULL;
}

bool isLowPoint(struct MapData *data, int y, int x) {
    int number = data->map[y][x];
    bool low = true;

    low = x == 0 || data->map[y][x - 1] > number;
    low = low && (x == data->width - 1 || data->map[y][x + 1] > number);
    low = low && (y == 0 || data->map[y - 1][x] > number);
    low = low && (y == data->height - 1 || data->map[y + 1][x] > number);

    return low;
}

void freeMapData(struct MapData *data) {
    for (int y = 0; y < data->height; y++) {
        free(data->map[y]);
    }

    free(data->map);
}