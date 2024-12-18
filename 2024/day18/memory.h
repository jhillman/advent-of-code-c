#include <stdlib.h>
#include <stdbool.h>

#define GRID_SIZE 71
#define MIN_BYTES 1024

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
    int steps;
};

bool equal(struct Location a, struct Location b) {
    return a.x == b.x && a.y == b.y;
}

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

void freeLocations(struct Locations *locations) {
    free(locations->data);
    free(locations);
}

struct LocationQueue {
    int size;
    int capacity;
    struct Locations **data;
    int fewestSteps;
};

void enqueue(struct LocationQueue *queue, struct Location location) {
    if (location.steps >= queue->capacity) {
        int capacity = queue->capacity;

        queue->capacity = location.steps + 1000;
        queue->data = (struct Locations **)realloc(queue->data, queue->capacity * sizeof(struct Locations *));

        for (int i = capacity; i < queue->capacity; i++) {
            queue->data[i] = (struct Locations *)calloc(1, sizeof(struct Locations));
        }
    }

    addLocation(queue->data[location.steps], location);

    if (!queue->fewestSteps || location.steps < queue->fewestSteps) {
        queue->fewestSteps = location.steps;
    }

    ++queue->size;
}

struct Location dequeue(struct LocationQueue *queue) {
    struct Locations *locations = queue->data[queue->fewestSteps];
    struct Location location = locations->data[locations->index++];

    --queue->size;
    
    if (locations->index == locations->count) {
        locations->index = 0;
        locations->count = 0;

        if (queue->size > 0) {
            for (int steps = queue->fewestSteps + 1; steps < queue->capacity; steps++) {
                if (queue->data[steps]->count > 0) {
                    queue->fewestSteps = steps;
                    break;
                }
            }
        } else {
            queue->fewestSteps = 0;
        }
    }

    return location;
}

void freeQueue(struct LocationQueue *queue) {
    for (int i = 0; i < queue->capacity; i++) {
        free(queue->data[i]->data);
        free(queue->data[i]);
    }

    free(queue->data);
    free(queue);
}

enum Type {
    SAFE,
    CORRUPTED
};

struct Memory {
    int width;
    int height;
    enum Type **grid;
    int ***steps;
    bool **visited;
    FILE *inputFile;
    struct Location start;
    struct Location end;
    struct Location lastByte;
};

int fewestSteps(struct Memory *memory) {
    int fewest = MAX_STEPS;

    struct LocationQueue *queue = (struct LocationQueue *)calloc(1, sizeof(struct LocationQueue));

    enum Direction directions[] = { UP, DOWN, LEFT, RIGHT };
    int xDeltas[] = {  0, 0, -1, 1 };
    int yDeltas[] = { -1, 1,  0, 0 };

    struct Location location = memory->start;

    enqueue(queue, location);

    for (int y = 0; y < memory->height; y++) {
        for (int x = 0; x < memory->width; x++) {
            memset(memory->steps[y][x], MAX_STEPS, 5 * sizeof(int));
        }

        memset(memory->visited[y], false, memory->width * sizeof(bool));
    }

    memory->steps[location.y][location.x][location.direction] = 0;

    while (queue->size) {
        location = dequeue(queue);

        memory->visited[location.y][location.x] = true;

        if (equal(location, memory->end) && location.steps < fewest) {
            fewest = location.steps;
        }

        if (location.steps > fewest) {
            continue;
        }

        for (int i = 0; i < 4; i++) {
            int x = location.x + xDeltas[i];
            int y = location.y + yDeltas[i];
            enum Direction direction = directions[i];

            if (!(x >= 0 && x < memory->width && y >= 0 && y < memory->height)) {
                continue;
            }

            if (memory->grid[y][x] == CORRUPTED) {
                continue;
            }

            int steps = location.steps + 1;

            if (memory->steps[y][x][direction] > steps) {
                memory->steps[y][x][direction] = steps;
                enqueue(queue, (struct Location){ 
                    x,
                    y,
                    direction, 
                    steps
                });
            }
        }
    }

    freeQueue(queue);

    return fewest;
}

struct Memory *getMemory(char *input) {
    FILE *inputFile = fopen(input, "r");
    struct Memory *memory = NULL;

    if (inputFile) {
        memory = calloc(1, sizeof(struct Memory));
        *memory = (struct Memory){ 
            GRID_SIZE, 
            GRID_SIZE, 
            (enum Type **)malloc(GRID_SIZE * sizeof(enum Type *)), 
            (int ***)malloc(GRID_SIZE * sizeof(int **)), 
            (bool **)malloc(GRID_SIZE * sizeof(bool *)),
            inputFile,
            { 0, 0, NONE, 0 },
            { GRID_SIZE - 1, GRID_SIZE - 1, NONE, 0 }
        };

        for (int y = 0; y < memory->height; y++) {
            memory->grid[y] = (enum Type *)malloc(memory->width * sizeof(enum Type));
            memory->steps[y] = (int **)malloc(memory->width * sizeof(int *));
            memory->visited[y] = (bool *)calloc(memory->width, sizeof(bool));

            for (int x = 0; x < memory->width; x++) {
                memory->grid[y][x] = SAFE;
                memory->steps[y][x] = (int *)malloc(5 * sizeof(int));
            }
        }
    }    

    return memory;    
}

void dropBytes(struct Memory *memory, int count) {
    int x;
    int y;

    while (count-- && !feof(memory->inputFile) && fscanf(memory->inputFile, "%d,%d", &x, &y)) {
        memory->grid[y][x] = CORRUPTED;
        memory->lastByte.x = x;
        memory->lastByte.y = y;
    }
}

void freeMemory(struct Memory *memory) {
    for (int y = 0; y < memory->height; y++) {
        for (int x = 0; x < memory->width; x++) {
            free(memory->steps[y][x]);
        }

        free(memory->grid[y]);
        free(memory->steps[y]);
        free(memory->visited[y]);
    }

    free(memory->grid);
    free(memory->steps);
    free(memory->visited);
    free(memory);
}