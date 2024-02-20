#include "../../aoc.h"
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

enum RegionType {
    ROCKY,
    WET,
    NARROW
};

enum Tool {
    NEITHER,
    TORCH,
    CLIMBING_GEAR
};

struct Location {
    int x;
    int y;
};

int timeBetween(struct Location start, struct Location end) {
    return abs(end.x - start.x) + abs(end.y - start.y);
}

struct SearchState {
    struct Location location;
    enum Tool tool;
    int time;
    int remainingTime;
};

struct SearchStates {
    int capacity;
    int count;
    int index;
    struct SearchState *data;
};

void addSearchState(struct SearchStates *states, struct SearchState state) {
    if (states->count == states->capacity) {
        states->capacity += 100;
        states->data = (struct SearchState *)realloc(states->data, states->capacity * sizeof(struct SearchState));
    }

    states->data[states->count++] = state;
}

void freeSearchStates(struct SearchStates *states) {
    free(states->data);
    free(states);
}

struct SearchStateQueue {
    int size;
    int capacity;
    struct SearchStates **data;
    int shortestTime;
};

void enqueue(struct SearchStateQueue *queue, struct SearchState state) {
    if (state.time >= queue->capacity) {
        int capacity = queue->capacity;

        queue->capacity = state.time + 1000;
        queue->data = (struct SearchStates **)realloc(queue->data, queue->capacity * sizeof(struct SearchStates *));

        for (int i = capacity; i < queue->capacity; i++) {
            queue->data[i] = (struct SearchStates *)calloc(1, sizeof(struct SearchStates));
        }
    }

    addSearchState(queue->data[state.time], state);

    if (!queue->shortestTime || state.time < queue->shortestTime) {
        queue->shortestTime = state.time;
    }

    ++queue->size;
}

struct SearchState dequeue(struct SearchStateQueue *queue) {
    struct SearchStates *states = queue->data[queue->shortestTime];
    struct SearchState state = states->data[states->index++];

    --queue->size;
    
    if (states->index == states->count) {
        states->index = 0;
        states->count = 0;

        if (queue->size > 0) {
            for (int time = queue->shortestTime + 1; time < queue->capacity; time++) {
                if (queue->data[time]->count > 0) {
                    queue->shortestTime = time;
                    break;
                }
            }
        } else {
            queue->shortestTime = 0;
        }
    }

    return state;
}

void freeQueue(struct SearchStateQueue *queue) {
    for (int i = 0; i < queue->capacity; i++) {
        free(queue->data[i]->data);
        free(queue->data[i]);
    }

    free(queue->data);
    free(queue);
}

struct Cave {
    int width;
    int height;
    int depth;
    struct Location mouth;
    struct Location target;

    struct SearchStateQueue *queue;
    bool ***visited;

    int **erosionLevels;
};

struct Cave *getCave(char *input) {
    FILE *inputFile = fopen(input, "r");
    struct Cave *cave = NULL;

    if (inputFile) {
        cave = (struct Cave *)calloc(1, sizeof(struct Cave));

        fscanf(inputFile, "depth: %d\ntarget: %d,%d", &cave->depth, &cave->target.x, &cave->target.y);

        fclose(inputFile);

        cave->width = cave->target.x + 1;
        cave->height = cave->target.y + 1;

        cave->erosionLevels = (int **)malloc(cave->height * sizeof(int *));

        for (int y = 0; y < cave->height; y++) {
            cave->erosionLevels[y] = (int *)calloc(cave->width, sizeof(int));
        }
    }

    return cave;
}

void freeCave(struct Cave *cave) {    
    for (int y = 0; y < cave->height; y++) {
        if (cave->visited) {
            for (int x = 0; x < cave->width; x++) {
                free(cave->visited[y][x]);
            }

            free(cave->visited[y]);
        }

        free(cave->erosionLevels[y]);
    }

    free(cave->erosionLevels);
    free(cave->visited);

    if (cave->queue) {
        freeQueue(cave->queue);
    }

    free(cave);
}

int erosionLevel(struct Cave *cave, struct Location location) {
    if (location.x == cave->width || location.y == cave->height) {
        int previousWidth = cave->width;
        int previousHeight = cave->height;

        if (location.x == cave->width) {
            cave->width += 100;
        }

        if (location.y == cave->height) {
            cave->height += 100;
            cave->erosionLevels = (int **)realloc(cave->erosionLevels, cave->height * sizeof(int *));
            cave->visited = (bool ***)realloc(cave->visited, cave->height * sizeof(bool **));
        }

        for (int y = 0; y < cave->height; y++) {
            if (y >= previousHeight) {
                cave->erosionLevels[y] = (int *)calloc(cave->width, sizeof(int));
                cave->visited[y] = (bool **)malloc(cave->width * sizeof(bool *));

                for (int x = 0; x < cave->width; x++) {
                    cave->visited[y][x] = (bool *)calloc(3, sizeof(bool));
                }
            } else if (cave->width > previousWidth) {
                cave->erosionLevels[y] = (int *)realloc(cave->erosionLevels[y], cave->width * sizeof(int));
                memset(cave->erosionLevels[y] + previousWidth, 0, (cave->width - previousWidth) * sizeof(int));

                cave->visited[y] = (bool **)realloc(cave->visited[y], cave->width * sizeof(bool *));

                for (int x = previousWidth; x < cave->width; x++) {
                    cave->visited[y][x] = (bool *)calloc(3, sizeof(bool));
                }
            }
        }
    }

    int level = cave->erosionLevels[location.y][location.x];

    if (!level) {
        if (location.x == 0 && location.y == 0 || location.x == cave->target.x && location.y == cave->target.y) {
            level = cave->depth;
        } else if (location.x == 0) {
            level = ((location.y * 48271) + cave->depth) % 20183;
        } else if (location.y == 0) {
            level = ((location.x * 16807) + cave->depth) % 20183;
        } else {
            level = ((erosionLevel(cave, (struct Location){ location.x, location.y - 1 }) * 
                      erosionLevel(cave, (struct Location){ location.x - 1, location.y })) + 
                      cave->depth) % 20183;
        }

        cave->erosionLevels[location.y][location.x] = level;
    }

    return level;
}

enum RegionType regionType(struct Cave *cave, struct Location location) {
    return (enum RegionType)erosionLevel(cave, location) % 3;
}

enum Tool changeTool(struct Cave *cave, struct SearchState state) {
    enum RegionType type = regionType(cave, state.location);
    
    switch (type) {
    case ROCKY:
        if (state.tool == CLIMBING_GEAR) {
            return TORCH;
        } else {
            return CLIMBING_GEAR;
        }
    case WET:
        if (state.tool == CLIMBING_GEAR) {
            return NEITHER;
        } else {
            return CLIMBING_GEAR;
        }
    case NARROW:
        if (state.tool == TORCH) {
            return NEITHER;
        } else {
            return TORCH;
        }
    }
}

bool validTool(enum RegionType type, enum Tool tool) {
    switch (type) {
    case ROCKY:
        return tool != NEITHER;
    case WET:
        return tool != TORCH;
    case NARROW:
        return tool != CLIMBING_GEAR;
    }
}