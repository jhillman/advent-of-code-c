#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Rock {
    int x;
    int y;
};

struct Rocks {
    int capacity;
    int count;
    struct Rock *data;
};

struct Reflector {
    int width;
    int height;
    char **platform;
    struct Rocks *rocks;
};

enum Direction {
    NORTH,
    SOUTH,
    EAST,
    WEST
};

void addRock(struct Rocks *rocks, struct Rock rock) {
    if (rocks->count == rocks->capacity) {
        rocks->capacity += 50;
        rocks->data = (struct Rock *)realloc(rocks->data, rocks->capacity * sizeof(struct Rock));
    }

    rocks->data[rocks->count++] = rock;
}

int compareAB(const void *a, const void *b) {
    int result = ((struct Rock *)a)->x - ((struct Rock *)b)->x;

    if (result == 0) {
        result = ((struct Rock *)a)->y - ((struct Rock *)b)->y;
    }

    return result;
}

int compareBA(const void *a, const void *b) {
    int result = ((struct Rock *)b)->x - ((struct Rock *)a)->x;

    if (result == 0) {
        result = ((struct Rock *)b)->y - ((struct Rock *)a)->y;
    }

    return result;
}

void tilt(struct Reflector *reflector, enum Direction direction, bool sort) {
    int xDelta = 0;
    int yDelta = 0;
    int (*compare)(const void *, const void *);

    switch (direction) {
    case NORTH:
        yDelta = -1;
        compare = compareAB;
        break;
    case WEST:
        xDelta = -1;
        compare = compareAB;
        break;
    case SOUTH:
        yDelta = 1;
        compare = compareBA;
        break;
    case EAST:
        xDelta = 1;
        compare = compareBA;
        break;
    }

    if (sort) {
        qsort(reflector->rocks->data, reflector->rocks->count, sizeof(struct Rock), compare);
    }

    for (int i = 0; i < reflector->rocks->count; i++) {
        struct Rock *rock = &reflector->rocks->data[i];
        int x = rock->x;
        int y = rock->y;

        while (x + xDelta >= 0 && x + xDelta < reflector->width && 
            y + yDelta >= 0 && y + yDelta < reflector->height && 
            reflector->platform[y + yDelta][x + xDelta] == '.') {
            reflector->platform[y + yDelta][x + xDelta] = 'O';
            reflector->platform[y][x] = '.';

            x += xDelta;
            y += yDelta;
        }

        rock->x = x;
        rock->y = y;
    }
}

void printReflector(struct Reflector *reflector) {
    for (int y = 0; y < reflector->height; y++) {
        for (int x = 0; x < reflector->width; x++) {
            printf("%c", reflector->platform[y][x]);
        }

        printf("\n");
    }
}

int totalLoad(struct Reflector *reflector) {
    int load = 0;

    for (int i = 0; i < reflector->rocks->count; i++) {
        load += (reflector->height - reflector->rocks->data[i].y);
    }

    return load;
}

struct Reflector *getReflector(char *input) {
    FILE *inputFile = fopen(input, "r");
    struct Reflector *reflector = NULL;

    if (inputFile) {
        char c;
        reflector = (struct Reflector *)calloc(1, sizeof(struct Reflector));
        reflector->rocks = (struct Rocks *)calloc(1, sizeof(struct Rocks));

        while (!feof(inputFile)) {
            c = fgetc(inputFile);

            if (c == '\n' || feof(inputFile)) {
                ++reflector->height;
            }

            if (!reflector->height) {
                ++reflector->width;
            }
        }

        fseek(inputFile, 0, SEEK_SET);

        reflector->platform = (char **)malloc(reflector->height * sizeof(char *));

        for (int y = 0; y < reflector->height; y++) {
            reflector->platform[y] = (char *)malloc(reflector->width * sizeof(char));

            for (int x = 0; x < reflector->width; x++) {
                c = fgetc(inputFile);

                if (c == 'O') {
                    addRock(reflector->rocks, (struct Rock) { x, y });
                }

                reflector->platform[y][x] = c;
            }

            fgetc(inputFile); // \n
        }

        fclose(inputFile);
    }

    return reflector;
}

void freeReflector(struct Reflector *reflector) {
    for (int y = 0; y < reflector->height; y++) {
        free(reflector->platform[y]);
    }

    free(reflector->platform);

    free(reflector->rocks->data);
    free(reflector->rocks);
    
    free(reflector);
}