#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_STEPS 10000

enum Direction {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct Location {
    int x;
    int y;
    enum Direction direction;
    int heatLoss;
    int steps;
};

struct Locations {
    int capacity;
    int count;
    int index;
    struct Location *data;
};

void addLocation(struct Locations *locations, struct Location location) {
    if (locations->count == locations->capacity) {
        locations->capacity += 100;
        locations->data = (struct Location *)realloc(locations->data, locations->capacity * sizeof(struct Location));
    }

    locations->data[locations->count++] = location;
}

struct LocationQueue {
    int capacity;
    int size;
    struct Locations **data;
    int lowestHeatLoss;
};

void enqueue(struct LocationQueue *queue, struct Location location) {
    if (!queue->capacity || location.heatLoss >= queue->capacity - 1) {
        queue->capacity += 1000;
        queue->data = (struct Locations **)realloc(queue->data, queue->capacity * sizeof(struct Locations *));

        for (int i = queue->capacity - 1000; i < queue->capacity; i++) {
            queue->data[i] = (struct Locations *)calloc(1, sizeof(struct Locations));
        }
    }

    addLocation(queue->data[location.heatLoss], location);
    
    if (!queue->lowestHeatLoss || location.heatLoss < queue->lowestHeatLoss) {
        queue->lowestHeatLoss = location.heatLoss;
    }

    ++queue->size;
}

struct Location dequeue(struct LocationQueue *queue) {
    struct Locations *locations = queue->data[queue->lowestHeatLoss];
    struct Location location = locations->data[locations->index++];

    if (locations->index == locations->count) {
        locations->index = 0;
        locations->count = 0;

        for (int heatLoss = queue->lowestHeatLoss; heatLoss < queue->capacity; heatLoss++) {
            if (queue->data[heatLoss]->count > 0) {
                queue->lowestHeatLoss = heatLoss;
                break;
            }
        }
    }

    --queue->size;

    return location;
}

void clear(struct LocationQueue *queue) {
    while(queue->size) {
        dequeue(queue);
    }    
}

void freeQueue(struct LocationQueue *queue) {
    for (int i = 0; i < queue->capacity; i++) {
        if (queue->data[i]) {
            free(queue->data[i]->data);
            free(queue->data[i]);
        }
    }

    free(queue->data);
    free(queue);
}

struct HeatLossData {
    int width;
    int height;
    int **values;
};

enum Direction opposite(enum Direction direction) {
    switch (direction) {
    case UP:
        return DOWN;
    case DOWN:
        return UP;
    case LEFT:
        return RIGHT;
    case RIGHT:
        return LEFT;
    default:
        return direction;
    }
}

bool equal(struct Location a, struct Location b) {
    return a.x == b.x && a.y == b.y;
}

int leastHeatLoss(struct HeatLossData *data, int minSteps, int maxSteps) {
    int ****cumulativeHeatLoss = (int ****)malloc(data->height * sizeof(int ***));

    for (int y = 0; y < data->height; y++) {
        cumulativeHeatLoss[y] = (int ***)malloc(data->width * sizeof(int **));

        for (int x = 0; x < data->width; x++) {
            cumulativeHeatLoss[y][x] = (int **)malloc(5 * sizeof(int *));

            for (int d = 0; d < 5; d++) {
                cumulativeHeatLoss[y][x][d] = (int *)malloc((maxSteps + 1) * sizeof(int));

                for (int s = 0; s <= maxSteps; s++) {
                    cumulativeHeatLoss[y][x][d][s] = MAX_STEPS;
                }
            }
        }
    }

    struct LocationQueue *queue = (struct LocationQueue *)calloc(1, sizeof(struct Location *));

    enum Direction directions[] = { UP, DOWN, LEFT, RIGHT };

    struct Location start = { 0, 0, NONE, 0, 0 };
    struct Location end = { data->width - 1, data->height - 1, NONE, 0, 0 };

    enqueue(queue, start);

    cumulativeHeatLoss[0][0][0][0] = 0;

    struct Location location;

    while (queue->size) {
        location = dequeue(queue);

        if (equal(location, end) && location.steps >= minSteps) {
            break;
        }

        for (int i = 0; i < 4; i++) {
            enum Direction direction = directions[i];

            if (direction && direction == opposite(location.direction)) {
                continue;
            }

            if (direction == location.direction && location.steps == maxSteps) {
                continue;
            }

            if (location.direction && direction != location.direction && location.steps < minSteps) {
                continue;
            }
            
            int x = location.x;
            int y = location.y;

            switch (direction) {
            case UP:
                --y;
                break;
            case DOWN:
                ++y;
                break;
            case LEFT:
                --x;
                break;
            case RIGHT:
                ++x;
                break;
            default:
                break;
            }

            if (!(x >= 0 && x < data->width && y >= 0 && y < data->height)) {
                continue;
            }

            int heatLoss = cumulativeHeatLoss[location.y][location.x][location.direction][location.steps] + data->values[y][x];
            int steps = (location.direction && location.direction == direction) ? location.steps + 1 : 1;

            if (cumulativeHeatLoss[y][x][direction][steps] > heatLoss) {
                cumulativeHeatLoss[y][x][direction][steps] = heatLoss;

                enqueue(queue, (struct Location){ x, y, direction, heatLoss, steps });
            }
        }
    }

    int heatLoss = cumulativeHeatLoss[end.y][end.x][location.direction][location.steps];

    for (int y = 0; y < data->height; y++) {
        for (int x = 0; x < data->width; x++) {
            for (int d = 0; d < 5; d++) {
                free(cumulativeHeatLoss[y][x][d]);
            }

            free(cumulativeHeatLoss[y][x]);
        }

        free(cumulativeHeatLoss[y]);
    }

    free(cumulativeHeatLoss);

    freeQueue(queue);

    return heatLoss;
}

struct HeatLossData *getHeatLossData(char *input) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        struct HeatLossData *data = (struct HeatLossData *)calloc(1, sizeof(struct HeatLossData));

        while (!feof(inputFile)) {
            char c = fgetc(inputFile);

            if (c == '\n' || feof(inputFile)) {
                ++data->height;
            }

            if (!data->height) {
                ++data->width;
            }
        }

        fseek(inputFile, 0, SEEK_SET);

        data->values = (int **)malloc(data->height * sizeof(int *));

        for (int y = 0; y < data->height; y++) {
            data->values[y] = (int *)malloc(data->width * sizeof(int));

            for (int x = 0; x < data->width; x++) {
                data->values[y][x] = fgetc(inputFile) - '0';
            }

            fgetc(inputFile); // \n
        }

        fclose(inputFile);

        return data;
    }

    return NULL;
}

void freeHeatLossData(struct HeatLossData *data) {
    for (int y = 0; y < data->height; y++) {
        free(data->values[y]);
    }

    free(data->values);
    free(data);
}