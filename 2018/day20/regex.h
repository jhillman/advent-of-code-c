#include "../../aoc.h"
#include <stdlib.h>
#include <stdbool.h>

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

struct Location {
    int x;
    int y;
};

struct LocationStack {
    int capacity;
    int count;
    struct Location *data;
};

void push(struct LocationStack *stack, struct Location location) {
    if (stack->count == stack->capacity) {
        stack->capacity += 10;
        stack->data = (struct Location *)realloc(stack->data, stack->capacity * sizeof(struct Location));
    }

    stack->data[stack->count++] = location;
}

struct Location peek(struct LocationStack *stack) {
    return stack->data[stack->count - 1];
}

struct Location pop(struct LocationStack *stack) {
    return stack->data[--stack->count];
}

void freeLocationStack(struct LocationStack *stack) {
    free(stack->data);
    free(stack);
}

void processRegex(FILE *inputFile, void (*processLocation)(struct Location, struct Location, void *), void *state) {
    struct LocationStack *stack = (struct LocationStack *)calloc(1, sizeof(struct LocationStack));

    struct Location location = { 0, 0 };
    struct Location lastLocation;
    bool process;

    fseek(inputFile, 0, SEEK_SET);

    while (!feof(inputFile)) {
        process = false;

        switch (fgetc(inputFile)) {
        case '(':
            push(stack, location);
            break;
        case '|':
            location = peek(stack);
            break;
        case ')':
            location = pop(stack);
            break;
        case 'N':
            --location.y;
            process = true;
            break;
        case 'S':
            ++location.y;
            process = true;
            break;
        case 'E':
            ++location.x;
            process = true;
            break;
        case 'W':
            --location.x;
            process = true;
            break;
        }

        if (process) {
            processLocation(location, lastLocation, state);
        }

        lastLocation = location;
    }

    freeLocationStack(stack);
}

struct LocationDistances {
    struct Location minimum;
    struct Location maximum;

    int width;
    int height;
    int **distances;

    int maxDistance;
    int distantLocations;
};

void processMinMax(struct Location location, struct Location lastLocation, void *state) {
    struct LocationDistances *locationDistances = (struct LocationDistances *)state;

    locationDistances->minimum.x = locationDistances->minimum.x ? min(locationDistances->minimum.x, location.x) : location.x;
    locationDistances->minimum.y = locationDistances->minimum.y ? min(locationDistances->minimum.y, location.y) : location.y;

    locationDistances->maximum.x = locationDistances->maximum.x ? max(locationDistances->maximum.x, location.x) : location.x;
    locationDistances->maximum.y = locationDistances->maximum.y ? max(locationDistances->maximum.y, location.y) : location.y;
}

void processLocationDistances(struct Location location, struct Location lastLocation, void *state) {
    struct LocationDistances *locationDistances = (struct LocationDistances *)state;

    location.x -= locationDistances->minimum.x;
    location.y -= locationDistances->minimum.y;

    lastLocation.x -= locationDistances->minimum.x;
    lastLocation.y -= locationDistances->minimum.y;

    int nextDistance = locationDistances->distances[lastLocation.y][lastLocation.x] + 1;
    int distance = locationDistances->distances[location.y][location.x];

    if (!distance || nextDistance < distance) {
        locationDistances->distances[location.y][location.x] = nextDistance;

        if (nextDistance >= 1000) {
            ++locationDistances->distantLocations;
        }

        locationDistances->maxDistance = max(locationDistances->maxDistance, nextDistance);
    }
}

struct LocationDistances *getLocationDistances(char *input) {
    FILE *inputFile = fopen(input, "r");
    struct LocationDistances *locationDistances = NULL;

    if (inputFile) {
        locationDistances = (struct LocationDistances *)calloc(1, sizeof(struct LocationDistances));

        processRegex(inputFile, processMinMax, locationDistances);

        locationDistances->width = (locationDistances->maximum.x - locationDistances->minimum.x) + 1;
        locationDistances->height = (locationDistances->maximum.y - locationDistances->minimum.y) + 1;

        locationDistances->distances = (int **)malloc(locationDistances->height * sizeof(int *));

        for (int y = 0; y < locationDistances->height; y++) {
            locationDistances->distances[y] = (int *)calloc(locationDistances->width, sizeof(int));
        }

        processRegex(inputFile, processLocationDistances, locationDistances);

        fclose(inputFile);
    }    

    return locationDistances;
}

void freeLocationDistances(struct LocationDistances *locationDistances) {
    for (int y = 0; y < locationDistances->height; y++) {
        free(locationDistances->distances[y]);
    }

    free(locationDistances->distances);
    free(locationDistances);
}