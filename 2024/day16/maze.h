#include <stdlib.h>
#include <stdbool.h>

#define MAX_SCORE 10000000

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
    int score;
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
    int lowestScore;
};

void enqueue(struct LocationQueue *queue, struct Location location) {
    if (location.score >= queue->capacity) {
        int capacity = queue->capacity;

        queue->capacity = location.score + 1000;
        queue->data = (struct Locations **)realloc(queue->data, queue->capacity * sizeof(struct Locations *));

        for (int i = capacity; i < queue->capacity; i++) {
            queue->data[i] = (struct Locations *)calloc(1, sizeof(struct Locations));
        }
    }

    addLocation(queue->data[location.score], location);

    if (!queue->lowestScore || location.score < queue->lowestScore) {
        queue->lowestScore = location.score;
    }

    ++queue->size;
}

struct Location dequeue(struct LocationQueue *queue) {
    struct Locations *locations = queue->data[queue->lowestScore];
    struct Location location = locations->data[locations->index++];

    --queue->size;
    
    if (locations->index == locations->count) {
        locations->index = 0;
        locations->count = 0;

        if (queue->size > 0) {
            for (int score = queue->lowestScore + 1; score < queue->capacity; score++) {
                if (queue->data[score]->count > 0) {
                    queue->lowestScore = score;
                    break;
                }
            }
        } else {
            queue->lowestScore = 0;
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
    PATH,
    WALL
};

struct Maze {
    int width;
    int height;
    enum Type **grid;
    int ***scores;
    bool **visited;

    struct Location start;
    struct Location end;
};

int lowestScore(struct Maze *maze) {
    int lowest = MAX_SCORE;

    struct LocationQueue *queue = (struct LocationQueue *)calloc(1, sizeof(struct LocationQueue));

    enum Direction directions[] = { NORTH, SOUTH, WEST, EAST };
    int xDeltas[] = {  0, 0, -1, 1 };
    int yDeltas[] = { -1, 1,  0, 0 };

    struct Location location = maze->start;

    enqueue(queue, location);

    for (int y = 0; y < maze->height; y++) {
        for (int x = 0; x < maze->width; x++) {
            if (maze->grid[y][x] == PATH) {
                for (int d = 0; d < 5; d++) {
                    maze->scores[y][x][d] = MAX_SCORE;
                }
            }
        }
    }

    maze->scores[location.y][location.x][location.direction] = 0;

    while (queue->size) {
        location = dequeue(queue);

        if (equal(location, maze->end) && location.score < lowest) {
            lowest = location.score;
        }

        if (location.score > lowest) {
            continue;
        }

        for (int i = 0; i < 4; i++) {
            int x = location.x + xDeltas[i];
            int y = location.y + yDeltas[i];
            enum Direction direction = directions[i];

            if (maze->grid[y][x] != PATH) {
                continue;
            }

            int dX = direction == location.direction ? x : location.x;
            int dY = direction == location.direction ? y : location.y;
            int score = location.score + (direction == location.direction ? 1 : 1000);

            if (maze->scores[dY][dX][direction] >= score) {
                maze->scores[dY][dX][direction] = score;
                enqueue(queue, (struct Location){ 
                    dX,
                    dY,
                    direction, 
                    score
                });
            }
        }
    }

    freeQueue(queue);

    return lowest;
}

void backtrackPath(struct Maze *maze, struct Location location, int pathScore) {
    if (pathScore == 0) {
        return;
    }

    maze->visited[location.y][location.x] = true;

    enum Direction directions[] = { NORTH, SOUTH, WEST, EAST };
    int xDeltas[] = {  0, 0, -1, 1 };
    int yDeltas[] = { -1, 1,  0, 0 };

    for (int i = 0; i < 4; i++) {
        int x = location.x + xDeltas[i];
        int y = location.y + yDeltas[i];
        enum Direction direction = directions[i];

        if (maze->grid[y][x] == WALL) {
            continue;
        }

        int dX = direction == location.direction ? x : location.x;
        int dY = direction == location.direction ? y : location.y;
        int score = pathScore - (direction == location.direction ? 1 : 1000);

        if (maze->scores[dY][dX][opposite(direction)] == score) {
            struct Location next = {
                dX, dY,
                direction
            };

            backtrackPath(maze, next, score);
        }
    }
}

int bestPathLocationCount(struct Maze *maze, int score) {
    struct Location start = maze->end;
    enum Direction directions[] = { NORTH, SOUTH, WEST, EAST };
    int count = 0;

    for (int i = 0; i < 4; i++) {
        start.direction = directions[i];

        backtrackPath(maze, start, score);
    }

    for (int y = 0; y < maze->height; y++) {
        for (int x = 0; x < maze->width; x++) {
            if (maze->visited[y][x]) {
                ++count;
            }
        }
    }

    return count;
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
        maze->scores = (int ***)malloc(maze->height * sizeof(int **));
        maze->visited = (bool **)malloc(maze->height * sizeof(bool *));

        for (int y = 0; y < maze->height; y++) {
            maze->grid[y] = (enum Type *)malloc(maze->width * sizeof(enum Type));
            maze->scores[y] = (int **)malloc(maze->width * sizeof(int *));
            maze->visited[y] = (bool *)calloc(maze->width, sizeof(bool));

            for (int x = 0; x < maze->width; x++) {
                c = fgetc(inputFile);

                switch (c) {
                case 'S':
                    maze->start = (struct Location){ x, y, EAST, 0 };
                    maze->grid[y][x] = PATH;
                    break;
                case 'E':
                    maze->end = (struct Location){ x, y, NONE, 0 };
                    maze->grid[y][x] = PATH;
                    break;
                case '#':
                    maze->grid[y][x] = WALL;
                    break;
                case '.':
                    maze->grid[y][x] = PATH;
                    break;
                }

                if (maze->grid[y][x] == PATH) {
                    maze->scores[y][x] = (int *)malloc(5 * sizeof(int));
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
        for (int x = 0; x < maze->width; x++) {
            if (maze->grid[y][x] == PATH) {
                free(maze->scores[y][x]);
            }
        }

        free(maze->grid[y]);
        free(maze->scores[y]);
        free(maze->visited[y]);
    }

    free(maze->grid);
    free(maze->scores);
    free(maze->visited);
    free(maze);
}
