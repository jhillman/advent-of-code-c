#include "../../aoc.h"
#include <stdlib.h>
#include <math.h>

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

#define INFINITE -7

struct Coordinates {
    int x;
    int y;
    int area;
};

struct CoordinatesList {
    int capacity;
    int count;
    struct Coordinates *data;

    struct Coordinates minimum;
    struct Coordinates maximum;
};

void addCoordinates(struct CoordinatesList *list, struct Coordinates coordinates) {
    if (list->count == list->capacity) {
        list->capacity += 10;
        list->data = (struct Coordinates *)realloc(list->data, list->capacity * sizeof(struct Coordinates));
    }

    list->data[list->count++] = coordinates;
}

void freeCoordinates(struct CoordinatesList *list) {
    free(list->data);
    free(list);
}

int distance(struct Coordinates one, struct Coordinates two) {
    return abs(one.x - two.x) + abs(one.y - two.y);
}

struct CoordinatesList *getCoordinates(char *input) {
    FILE *inputFile = fopen(input, "r");
    struct CoordinatesList *list = NULL;

    if (inputFile) {
        list = (struct CoordinatesList *)calloc(1, sizeof(struct CoordinatesList));
        struct Coordinates coordinates = { 0, 0, 0 };

        while (fscanf(inputFile, "%d, %d\n", &coordinates.x, &coordinates.y) == 2) {
            addCoordinates(list, coordinates);

            list->minimum.x = list->minimum.x ? min(list->minimum.x, coordinates.x) : coordinates.x;
            list->minimum.y = list->minimum.y ? min(list->minimum.y, coordinates.y) : coordinates.y;

            list->maximum.x = list->maximum.x ? max(list->maximum.x, coordinates.x) : coordinates.x;
            list->maximum.y = list->maximum.y ? max(list->maximum.y, coordinates.y) : coordinates.y;
        }

        fclose(inputFile);
    }

    return list;
}