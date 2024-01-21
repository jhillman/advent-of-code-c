#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_MINUTES 1000

enum State {
    EMPTY = 0,
    WALL  = 1,
    NORTH = 1 << 1,
    SOUTH = 1 << 2,
    EAST  = 1 << 3,
    WEST  = 1 << 4
};

struct Location {
    int x;
    int y;
    int minute;
};

bool equal(struct Location a, struct Location b) {
    return a.x == b.x && a.y == b.y;
}

struct LocationQueue {
    struct Location *data;
    int capacity;
    int size;
    int index;
};

void addLocation(struct LocationQueue *queue, struct Location location) {
    if (queue->capacity == 0) {
        queue->capacity = 10000;
        queue->data = (struct Location *)malloc(queue->capacity * sizeof(struct Location));
    } else if (queue->size == queue->capacity) {
        queue->capacity += 10000;
        queue->data = (struct Location *)realloc(queue->data, queue->capacity * sizeof(struct Location));
    }

    queue->data[queue->size++] = location;    
}

struct Location removeLocation(struct LocationQueue *queue) {
    return queue->data[queue->index++];
}

int **emptyValley(int width, int height) {
    int **valley = (int **)calloc(height, sizeof(int *));

    for (int y = 0; y < height; y++) {
        valley[y] = (int *)calloc(width, sizeof(int));
    }

    return valley;
}

void freeValley(int **valley, int height) {
    for (int y = 0; y < height; y++) {
        free(valley[y]);
    }

    free(valley);
}

void printValley(int **valley, int width, int height, int expeditionX, int expeditionY) {
    int directionCount;
    char character;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x == expeditionX && y == expeditionY) {
                printf("E");
            } else {
                switch (valley[y][x]) {
                case EMPTY:
                    printf(".");
                    break;
                case WALL:
                    printf("#");
                    break;
                default:
                    directionCount = 0;
                    character = ' ';

                    if (valley[y][x] & NORTH) {
                        character = '^';
                        ++directionCount;
                    }

                    if (valley[y][x] & SOUTH) {
                        character = 'v';
                        ++directionCount;
                    }

                    if (valley[y][x] & EAST) {
                        character = '>';
                        ++directionCount;
                    }

                    if (valley[y][x] & WEST) {
                        character = '<';
                        ++directionCount;
                    }

                    printf("%c", directionCount > 1 ? '0' + directionCount : character);
                }
            }
        }

        printf("\n");
    }
}

void updateBlizzard(enum State blizzard, int x, int y, int **valley, int width, int height) {
    if (x == 0) {
        updateBlizzard(blizzard, width - 2, y, valley, width, height);
    } else if (x == width - 1) {
        updateBlizzard(blizzard, 1, y, valley, width, height);
    } else if (y == 0) {
        updateBlizzard(blizzard, x, height - 2, valley, width, height);
    } else if (y == height - 1) {
        updateBlizzard(blizzard, x, 1, valley, width, height);
    } else {
        valley[y][x] |= blizzard;
    }
}

int **updateValley(int **valley, int width, int height) {
    int **updatedValley = emptyValley(width, height);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            switch (valley[y][x]) {
            case EMPTY:
                break;
            case WALL:
                updatedValley[y][x] = valley[y][x];
                break;
            default:
                if (valley[y][x] & NORTH) {
                    updateBlizzard(NORTH, x, y - 1, updatedValley, width, height);
                }

                if (valley[y][x] & SOUTH) {
                    updateBlizzard(SOUTH, x, y + 1, updatedValley, width, height);
                }

                if (valley[y][x] & EAST) {
                    updateBlizzard(EAST, x + 1, y, updatedValley, width, height);
                }

                if (valley[y][x] & WEST) {
                    updateBlizzard(WEST, x - 1, y, updatedValley, width, height);
                }
            }
        }
    }

    return updatedValley;
}

int gcd(int one, int two) {
    if (one == 0) {
        return two;
    }

    return gcd(two % one, one);
}

int lcm(int one, int two) {
    return one * two / gcd(one, two);
}

struct ValleyData {
    int width;
    int height;
    int ***valleys;
    int valleyCount;
    struct Location start;
    struct Location end;
};

struct ValleyData *getValleyData(char *input) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        struct ValleyData *data = (struct ValleyData *)calloc(1, sizeof(struct ValleyData));

        char c = fgetc(inputFile);

        while (!feof(inputFile)) {
            if (c == '\n') {
                ++data->height;
            }

            if (!data->height) {
                ++data->width;
            }

            c = fgetc(inputFile);
        }

        ++data->height;

        fseek(inputFile, 0, SEEK_SET);

        int **valley = emptyValley(data->width, data->height);

        for (int y = 0; y < data->height; y++) {
            for (int x = 0; x < data->width; x++) {
                switch (fgetc(inputFile)) {
                case '.':
                    if (y == 0) {
                        data->start = (struct Location){ x, y, 0 };
                    } else if (y == data->height - 1) {
                        data->end = (struct Location){ x, y, 0 };
                    }

                    valley[y][x] = EMPTY;
                    break;
                case '#':
                    valley[y][x] = WALL;
                    break;
                case '^':
                    valley[y][x] = NORTH;
                    break;
                case 'v':
                    valley[y][x] = SOUTH;
                    break;
                case '>':
                    valley[y][x] = EAST;
                    break;
                case '<':
                    valley[y][x] = WEST;
                    break;
                }
            }

            if (y < data->height) {
                fgetc(inputFile); // \n
            }
        }

        fclose(inputFile);

        data->valleyCount = lcm(data->width - 2, data->height - 2);

        data->valleys = (int ***)malloc(data->valleyCount * sizeof(int **));
        *data->valleys = valley;

        for (int i = 1; i < data->valleyCount; i++) {
            data->valleys[i] = updateValley(data->valleys[i - 1], data->width, data->height);
        }

        return data;
    }

    return NULL;
}

void freeValleyData(struct ValleyData *data) {
    free(data->valleys);
    free(data);
}

int fewestMinutes(struct ValleyData *data, int minute, struct Location start, struct Location end) {
    struct LocationQueue *queue = (struct LocationQueue *)calloc(1, sizeof(struct LocationQueue));

    int xDeltas[] = { 0, -1, 0, 1, 0 };
    int yDeltas[] = { 0, 0, 1, 0, -1 };

    bool ***visitedLocations = (bool ***)malloc(data->height * sizeof(bool **));

    for (int y = 0; y < data->height; y++) {
        visitedLocations[y] = (bool **)malloc(data->width * sizeof(bool *));

        for (int x = 0; x < data->width; x++) {
            visitedLocations[y][x] = (bool *)calloc(MAX_MINUTES, sizeof(bool));
        }
    }

    addLocation(queue, start);

    int minutes;

    while (queue->size) {
        struct Location location = removeLocation(queue);

        if (equal(location, end)) {
            minutes = location.minute;
            break;
        }

        if (visitedLocations[location.y][location.x][location.minute]) {
            continue;
        }

        visitedLocations[location.y][location.x][location.minute] = true;

        int **valley = data->valleys[(location.minute + 1 + minute) % data->valleyCount];

        for (int i = 0; i < 5; i++) {
            int x = location.x + xDeltas[i];
            int y = location.y + yDeltas[i];

            if (!(x >= 0 && x < data->width && y >= 0 && y < data->height)) {
                continue;
            }

            if (valley[y][x] == EMPTY) {
                addLocation(queue, (struct Location){ x, y, location.minute + 1 });
            }
        }
    }

    for (int y = 0; y < data->height; y++) {
        for (int x = 0; x < data->width; x++) {
            free(visitedLocations[y][x]);
        }

        free(visitedLocations[y]);
    }

    free(visitedLocations);

    free(queue->data);
    free(queue);

    return minutes;
}