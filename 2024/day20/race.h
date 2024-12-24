#include <stdlib.h>
#include <stdbool.h>

#define MAX_TIME 10000000

enum Direction {
    NONE,
    NORTH,
    SOUTH,
    WEST,
    EAST
};

enum Direction opposite(enum Direction direction) {
    switch (direction) {
    case NORTH:
        return SOUTH;
    case SOUTH:
        return NORTH;
    case WEST:
        return EAST;
    case EAST:
        return WEST;
    default:
        return direction;
    }
}

struct Location {
    int x;
    int y;
    enum Direction direction;
    int time;
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
    int fastestTime;
};

void enqueue(struct LocationQueue *queue, struct Location location) {
    if (location.time >= queue->capacity) {
        int capacity = queue->capacity;

        queue->capacity = location.time + 100;
        queue->data = (struct Locations **)realloc(queue->data, queue->capacity * sizeof(struct Locations *));

        for (int i = capacity; i < queue->capacity; i++) {
            queue->data[i] = (struct Locations *)calloc(1, sizeof(struct Locations));
        }
    }

    addLocation(queue->data[location.time], location);

    if (!queue->fastestTime || location.time < queue->fastestTime) {
        queue->fastestTime = location.time;
    }

    ++queue->size;
}

struct Location dequeue(struct LocationQueue *queue) {
    struct Locations *locations = queue->data[queue->fastestTime];
    struct Location location = locations->data[locations->index++];

    --queue->size;
    
    if (locations->index == locations->count) {
        locations->index = 0;
        locations->count = 0;

        if (queue->size > 0) {
            for (int time = queue->fastestTime + 1; time < queue->capacity; time++) {
                if (queue->data[time]->count > 0) {
                    queue->fastestTime = time;
                    break;
                }
            }
        } else {
            queue->fastestTime = 0;
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
    TRACK,
    WALL
};

struct Maze {
    int width;
    int height;
    enum Type **grid;
    int **times;
    struct Locations *track;

    struct Location start;
    struct Location end;
};

void race(struct Maze *maze) {
    int fastest = MAX_TIME;

    struct LocationQueue *queue = (struct LocationQueue *)calloc(1, sizeof(struct LocationQueue));

    enum Direction directions[] = { NORTH, SOUTH, WEST, EAST };
    int xDeltas[] = {  0, 0, -1, 1 };
    int yDeltas[] = { -1, 1,  0, 0 };

    struct Location location = maze->start;

    enqueue(queue, location);

    for (int y = 0; y < maze->height; y++) {
        for (int x = 0; x < maze->width; x++) {
            maze->times[y][x] = MAX_TIME;
        }
    }

    maze->times[location.y][location.x] = 0;

    while (queue->size) {
        location = dequeue(queue);

        addLocation(maze->track, location);

        if (equal(location, maze->end) && location.time < fastest) {
            maze->end.direction = location.direction;
            fastest = location.time;
        }

        if (location.time > fastest) {
            continue;
        }

        for (int i = 0; i < 4; i++) {
            int x = location.x + xDeltas[i];
            int y = location.y + yDeltas[i];
            enum Direction direction = directions[i];

            if (maze->grid[y][x] != TRACK) {
                continue;
            }

            int time = location.time + 1;

            if (maze->times[y][x] >= time) {
                maze->times[y][x] = time;
                enqueue(queue, (struct Location){ 
                    x,
                    y,
                    direction, 
                    time
                });
            }
        }
    }

    freeQueue(queue);
}

struct Maze *getMaze(char *input) {
    FILE *inputFile = fopen(input, "r");
    struct Maze *maze = NULL;

    if (inputFile) {
        maze = (struct Maze *)calloc(1, sizeof(struct Maze));
        char c;

        while (!feof(inputFile)) {
            c = fgetc(inputFile);

            if (c == '\n' || feof(inputFile)) {
                ++maze->height;
            }

            if (!maze->height) {
                ++maze->width;
            }
        }

        fseek(inputFile, 0, SEEK_SET);

        maze->grid = (enum Type **)malloc(maze->height * sizeof(enum Type *));
        maze->times = (int **)malloc(maze->height * sizeof(int *));
        maze->track = (struct Locations *)calloc(1, sizeof(struct Locations));

        for (int y = 0; y < maze->height; y++) {
            maze->grid[y] = (enum Type *)malloc(maze->width * sizeof(enum Type));
            maze->times[y] = (int *)malloc(maze->width * sizeof(int));

            for (int x = 0; x < maze->width; x++) {
                c = fgetc(inputFile);

                switch (c) {
                case 'S':
                    maze->start = (struct Location){ x, y, EAST, 0 };
                    maze->grid[y][x] = TRACK;
                    break;
                case 'E':
                    maze->end = (struct Location){ x, y, NONE, 0 };
                    maze->grid[y][x] = TRACK;
                    break;
                case '#':
                    maze->grid[y][x] = WALL;
                    break;
                case '.':
                    maze->grid[y][x] = TRACK;
                    break;
                }
            }

            fgetc(inputFile); // \n
        }

        fclose(inputFile);
    }

    return maze;
}

void freeMaze(struct Maze *maze) {
    for (int y = 0; y < maze->height; y++) {
        free(maze->grid[y]);
        free(maze->times[y]);
    }

    freeLocations(maze->track);
    free(maze->grid);
    free(maze->times);
    free(maze);
}