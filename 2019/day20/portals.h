#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STEPS 10000

enum Direction {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT
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

struct Location {
    int x;
    int y;
    enum Direction direction;
    int level;
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

enum TileType {
    SPACE,
    WALL,
    PATH
};

struct Tile {
    struct Location location;
    char character;
    enum TileType type;
    int portalId;
};

enum NameOrientation {
    HORIZONTAL,
    VERTICAL
};

enum PortalType {
    OUTSIDE,
    INSIDE
};

struct Portal {
    int id;
    struct Location location;
    char name[3];
    enum PortalType type;
};

struct Portals {
    int count;
    struct Portal *data;
};

struct Maze {
    int width;
    int height;

    struct Location start;
    struct Location end;

    struct Tile **tiles;
    struct Portals *portals;
};

void handleMazeName(struct Maze *maze, char name[3], struct Location location, enum NameOrientation orientation) {
    enum PortalType type;

    switch (orientation) {
    case HORIZONTAL:
        if (location.x == 2) {
            location.direction = RIGHT;
            type = OUTSIDE;
        } else if (location.x == maze->width - 1) {
            location.x -= 2;
            location.direction = LEFT;
            type = OUTSIDE;
        } else if (maze->tiles[location.y][location.x].type == PATH) {
            location.direction = RIGHT;
            type = INSIDE;
        } else {
            location.x -= 3;
            location.direction = LEFT;
            type = INSIDE;
        }
        break;
    case VERTICAL:
        if (location.y == 2) {
            location.direction = DOWN;
            type = OUTSIDE;
        } else if (location.y == maze->height - 1) {
            location.y -= 2;
            location.direction = UP;
            type = OUTSIDE;
        } else if (maze->tiles[location.y][location.x].type == PATH) {
            location.direction = DOWN;
            type = INSIDE;
        } else {
            location.y -= 3;
            location.direction = UP;
            type = INSIDE;
        }
        break;
    }

    name[2] = '\0';

    if (strcmp(name, "AA") == 0) {
        location.level = 0;
        maze->start = location;
    } else if (strcmp(name, "ZZ") == 0) {
        location.level = 0;
        maze->end = location;
    } else {
        struct Portal portal = { maze->portals->count, location };
        strcpy(portal.name, name);
        portal.type = type;

        for (int i = 1; i < maze->portals->count; i++) {
            if (strcmp(maze->portals->data[i].name, portal.name) == 0) {
                struct Portal other = maze->portals->data[i];

                maze->tiles[other.location.y][other.location.x].portalId = portal.id;
                maze->tiles[portal.location.y][portal.location.x].portalId = other.id;

                break;
            }
        }

        maze->portals->data = (struct Portal *)realloc(maze->portals->data, (maze->portals->count + 1) * sizeof(struct Portal));
        maze->portals->data[maze->portals->count++] = portal;
    }
}

struct Maze *getMaze(char *input) {
    FILE *inputFile = fopen(input, "r");
    struct Maze *maze = NULL;

    if (inputFile) {
        char c;
        char name[3];
        struct Location location;
        struct Tile tile;
        int nameLength = 0;

        maze = (struct Maze *)calloc(1, sizeof(struct Maze));

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

        maze->tiles = (struct Tile **)malloc(maze->height * sizeof(struct Tile *));
        maze->portals = (struct Portals *)calloc(1, sizeof(struct Portals));
        maze->portals->data = (struct Portal *)malloc(sizeof(struct Portal));
        maze->portals->count = 1;

        for (int y = 0; y < maze->height; y++) {
            maze->tiles[y] = (struct Tile *)calloc(maze->width, sizeof(struct Tile));

            for (int x = 0; x < maze->width; x++) {
                location = (struct Location){ x, y };
                tile = (struct Tile){ location, fgetc(inputFile) };

                if (tile.character == '.') {
                    tile.type = PATH;
                } else if (tile.character == '#') {
                    tile.type = WALL;
                } else {
                    tile.type = SPACE;
                }

                maze->tiles[y][x] = tile;

                if (isalpha(tile.character)) {
                    name[nameLength++] = tile.character;
                } else {
                    if (nameLength == 2) {
                        handleMazeName(maze, name, location, HORIZONTAL);
                    }

                    nameLength = 0;
                }
            }

            if (nameLength == 2) {
                handleMazeName(maze, name, location, HORIZONTAL);
            }

            nameLength = 0;

            fgetc(inputFile); // \n
        }

        fclose(inputFile);

        for (int x = 0; x < maze->width; x++) {
            for (int y = 0; y < maze->height; y++) {
                location = (struct Location){ x, y };
                struct Tile tile = maze->tiles[y][x];

                if (isalpha(tile.character)) {
                    name[nameLength++] = tile.character;
                } else {
                    if (nameLength == 2) {
                        handleMazeName(maze, name, location, VERTICAL);
                    }

                    nameLength = 0;
                }
            }

            if (nameLength == 2) {
                handleMazeName(maze, name, location, VERTICAL);
            }

            nameLength = 0;
        }

        bool blockingDeadEnds = true;
        int xDeltas[] = {  0, 0, -1, 1 };
        int yDeltas[] = { -1, 1,  0, 0 };
        int walls;

        while (blockingDeadEnds) {
            blockingDeadEnds = false;

            for (int y = 0; y < maze->height; y++) {
                for (int x = 0; x < maze->width; x++) {
                    if (maze->tiles[y][x].type == PATH) {
                        walls = 0;

                        for (int i = 0; i < 4; i++) {
                            walls += (maze->tiles[y + yDeltas[i]][x + xDeltas[i]].type == WALL ? 1 : 0);
                        }

                        if (walls >= 3) {
                            maze->tiles[y][x].type = WALL;
                            maze->tiles[y][x].character = '#';

                            blockingDeadEnds = true;
                        }
                    }
                }
            }
        }
    }

    return maze;
}

void freeMaze(struct Maze *maze) {
    for (int y = 0; y < maze->height; y++) {
        free(maze->tiles[y]);
    }

    free(maze->tiles);

    if (maze->portals) {
        free(maze->portals->data);
        free(maze->portals);
    }
}

int fewestSteps(struct Maze *maze, bool recursing) {
    int levels = recursing ? 150 : 1;
    int steps = 0;

    int ****cumulativeSteps = (int ****)malloc(maze->height * sizeof(int ***));

    for (int y = 0; y < maze->height; y++) {
        cumulativeSteps[y] = (int ***)malloc(maze->width * sizeof(int **));

        for (int x = 0; x < maze->width; x++) {
            if (maze->tiles[y][x].type == PATH) {
                cumulativeSteps[y][x] = (int **)malloc(levels * sizeof(int *));

                for (int l = 0; l < levels; l++) {
                    cumulativeSteps[y][x][l] = (int *)malloc(5 * sizeof(int));
        
                    for (int d = 0; d < 5; d++) {
                        cumulativeSteps[y][x][l][d] = MAX_STEPS;
                    }
                }
            }
        }
    }

    struct LocationQueue *queue = (struct LocationQueue *)calloc(1, sizeof(struct LocationQueue));

    enum Direction directions[] = { UP, DOWN, LEFT, RIGHT };
    int xDeltas[] = {  0, 0, -1, 1 };
    int yDeltas[] = { -1, 1,  0, 0 };

    struct Location location = maze->start;

    enqueue(queue, location);

    cumulativeSteps[location.y][location.x][location.level][location.direction] = 0;

    while (queue->size) {
        location = dequeue(queue);

        if (equal(location, maze->end) && location.level == 0) {
            steps = location.steps;
            break;
        }

        struct Portal portal = maze->portals->data[maze->tiles[location.y][location.x].portalId];
        int level = location.level + (recursing ? (portal.type == OUTSIDE ? 1 : -1) : 0);

        if (portal.id && level >= 0) {
            int *steps = &cumulativeSteps[portal.location.y][portal.location.x][level][portal.location.direction];

            if (*steps > location.steps + 1) {
                *steps = location.steps + 1;

                enqueue(queue, (struct Location){ 
                    portal.location.x, 
                    portal.location.y, 
                    portal.location.direction,
                    level, 
                    location.steps + 1 
                });
            }
        }

        for (int i = 0; i < 4; i++) {
            int x = location.x + xDeltas[i];
            int y = location.y + yDeltas[i];
            enum Direction direction = directions[i];

            if (maze->tiles[y][x].type != PATH) {
                continue;
            }

            int *steps = &cumulativeSteps[y][x][location.level][direction];

            if (*steps > location.steps + 1) {
                *steps = location.steps + 1;

                enqueue(queue, (struct Location){ 
                    x, 
                    y, 
                    direction, 
                    location.level, 
                    location.steps + 1 
                });
            }
        }
    }

    for (int y = 0; y < maze->height; y++) {
        for (int x = 0; x < maze->width; x++) {
            if (maze->tiles[y][x].type == PATH) {
                for (int l = 0; l < levels; l++) {
                    free(cumulativeSteps[y][x][l]);
                }

                free(cumulativeSteps[y][x]);
            }
        }

        free(cumulativeSteps[y]);
    }

    free(cumulativeSteps);

    freeQueue(queue);

    return steps;
}