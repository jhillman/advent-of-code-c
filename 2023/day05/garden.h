#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

struct Seeds {
    int count;
    long *data;
};

struct Range {
    long source;
    long destination;
    long length;
};

struct Ranges {
    int capacity;
    int count;
    struct Range *data;
};

int compareRanges(struct Range rangeOne, struct Range rangeTwo) {
    long result = rangeOne.source - rangeTwo.source;

    if (result > 0) {
        return 1;
    } else if (result < 0) {
        return -1;
    }

    return 0;
}

struct Range *binarySearchRanges(struct Ranges *ranges, long value, int low, int high) {
    if (low > high) {
        return NULL;
    }

    int middle = low + ((high - low) / 2);

    if (value >= ranges->data[middle].source && value < ranges->data[middle].source + ranges->data[middle].length) {
        return &ranges->data[middle];
    }

    if (ranges->data[middle].source - value > 0) {
        return binarySearchRanges(ranges, value, low, middle - 1);
    } else {
        return binarySearchRanges(ranges, value, middle + 1, high);
    }
}

int compare(const void *a, const void *b) {
    return compareRanges(*(struct Range *)a, *(struct Range *)b);
}

void addRange(struct Ranges *ranges, long source, long destination, long length, bool reverse) {
    if (ranges->count == ranges->capacity) {
        ranges->capacity += 5;
        ranges->data = (struct Range *)realloc(ranges->data, ranges->capacity * sizeof(struct Range));
    }

    ranges->data[ranges->count++] = (struct Range) { reverse ? destination : source, reverse ? source : destination, length };
}

struct Map {
    char name[32];
    struct Ranges *ranges;
    struct Map *next;
};

struct Garden {
    struct Seeds *seeds;
    struct Map *map;
};

long mapValue(long value, struct Ranges *ranges) {
    long mappedValue = value;
    struct Range *range = binarySearchRanges(ranges, value, 0, ranges->count);

    if (range) {
        mappedValue += range->destination - range->source;
    }

    return mappedValue;
}

long processValue(struct Garden *garden, long value) {
    struct Map *map = garden->map;

    while (map) {
        value = mapValue(value, map->ranges);

        map = map->next;
    }

    return value;
}

int digitCount(long number) {
    return (int)floor(log10((double)number)) + 1;
}

struct Garden *getGarden(char *input, bool reverse) {
    FILE *inputFile = fopen(input, "r");
    struct Garden *garden = NULL;

    if (inputFile) {
        int seedLineLength = 1;
        char *seedLine;
        char *seedPosition;
        long seed;
        struct Map *map = NULL;
        char name[32];
        long destination;
        long source;
        long length;

        do {
            ++seedLineLength;
        } while (fgetc(inputFile) != '\n');

        seedLine = (char *)malloc(seedLineLength);

        fseek(inputFile, 0, SEEK_SET);

        fgets(seedLine, seedLineLength, inputFile);
        seedLine[strcspn(seedLine, "\n")] = 0;

        seedPosition = seedLine + 7;

        garden = (struct Garden *)calloc(1, sizeof(struct Garden));
        garden->seeds = (struct Seeds *)calloc(1, sizeof(struct Seeds));

        while (seedPosition < seedLine + strlen(seedLine) && sscanf(seedPosition, "%ld", &seed)) {
            garden->seeds->data = (long *)realloc(garden->seeds->data, (garden->seeds->count + 1) * sizeof(long));
            garden->seeds->data[garden->seeds->count++] = seed;

            seedPosition += digitCount(seed) + 1;
        }

        free(seedLine);

        while (!feof(inputFile) && fscanf(inputFile, "%s map:", name)) {
            if (reverse) {
                garden->map = map;
                map = (struct Map *)calloc(1, sizeof(struct Map));
                map->next = garden->map;
                garden->map = map;
            } else if (!garden->map) {
                garden->map = (struct Map *)calloc(1, sizeof(struct Map));
                map = garden->map;
            } else {
                map->next = (struct Map *)calloc(1, sizeof(struct Map));
                map = map->next;
            }

            strcpy(map->name, name);
            map->ranges = (struct Ranges *)calloc(1, sizeof(struct Ranges));

            while (!feof(inputFile) && fscanf(inputFile, "%ld %ld %ld\n", &destination, &source, &length)) {
                addRange(map->ranges, source, destination, length, reverse);
            }

            qsort(map->ranges->data, map->ranges->count, sizeof(struct Range), compare);
        }

        fclose(inputFile);
    }

    return garden;
}

void freeMap(struct Map *map) {
    if (map->next) {
        freeMap(map->next);
    }

    free(map->ranges->data);
    free(map->ranges);
    free(map);
}

void freeGarden(struct Garden *garden) {
    free(garden->seeds->data);
    free(garden->seeds);
    freeMap(garden->map);
    free(garden);
}
