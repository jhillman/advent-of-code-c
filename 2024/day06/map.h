#include <stdlib.h>
#include <stdbool.h>

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct Position {
    int x;
    int y;
};

enum Type {
    OUT,
    PATH,
    OBSTRUCTION,
    LOOP
};

struct MapPosition {
    enum Type type;
    bool visited;
    bool directions[4];
};

struct Map {
    int width;
    int height;
    struct MapPosition **grid;
    
    struct Position position;
    enum Direction direction;

    struct Position originalPosition;
    enum Direction originalDirection;
    
    int visited;
};

struct Positions {
    int capacity;
    int count;
    struct Position *data;
};

void addPosition(struct Positions *positions, struct Position position) {
    if (positions == NULL) {
        return;
    }

    if (positions->count == positions->capacity) {
        positions->capacity += 10;
        positions->data = (struct Position *)realloc(positions->data, positions->capacity * sizeof(struct Position));
    }

    positions->data[positions->count++] = position;
}

void freePositions(struct Positions *positions) {
    free(positions->data);
    free(positions);
}

void turn(struct Map *map) {
    switch (map->direction) {
    case UP:
        map->direction = RIGHT;
        break;
    case DOWN:
        map->direction = LEFT;
        break;
    case LEFT:
        map->direction = UP;
        break;
    case RIGHT:
        map->direction = DOWN;
        break;
    default:
        break;
    }
}

void stepPosition(struct Position *position, enum Direction direction) {
    switch (direction) {
    case UP:
        --position->y;
        break;
    case DOWN:
        ++position->y;
        break;
    case LEFT:
        --position->x;
        break;
    case RIGHT:
        ++position->x;
        break;
    default:
        break;
    }    
}

enum Type nextStep(struct Map *map, struct Positions *positions) {
    struct Position next = map->position;

    if (!map->grid[map->position.y][map->position.x].visited) {
        map->grid[map->position.y][map->position.x].visited = true;
        addPosition(positions, map->position);
        ++map->visited;
    }

    map->grid[map->position.y][map->position.x].directions[map->direction] = true;

    stepPosition(&next, map->direction);

    if (next.y < 0 || next.y >= map->height || 
        next.x < 0 || next.x >= map->width) {
        return OUT;
    }

    if (map->grid[next.y][next.x].directions[map->direction]) {
        return LOOP;
    }

    return map->grid[next.y][next.x].type;
}

enum Type patrol(struct Map *map, struct Positions *positions) {
    enum Type next = nextStep(map, positions);

    while (next != OUT && next != LOOP) {
        if (next == OBSTRUCTION) {
            turn(map);
        } else {
            stepPosition(&map->position, map->direction);
        }

        next = nextStep(map, positions);
    }

    return next;
}

void resetMap(struct Map *map) {
    map->position = map->originalPosition;
    map->direction = map->originalDirection;

    for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) {
            map->grid[y][x].directions[UP] = false;
            map->grid[y][x].directions[DOWN] = false;
            map->grid[y][x].directions[LEFT] = false;
            map->grid[y][x].directions[RIGHT] = false;
        }
    }
}

struct Map *getMap(char *input) {
    FILE *inputFile = fopen(input, "r");
    struct Map *map = NULL;

    if (inputFile) {
        char c;
        
        map = (struct Map *)calloc(1, sizeof(struct Map));

        while (!feof(inputFile)) {
            c = fgetc(inputFile);

            if (c == '\n' || feof(inputFile)) {
                ++map->height;
            }

            if (!map->height) {
                ++map->width;
            }
        }

        fseek(inputFile, 0, SEEK_SET);

        map->grid = (struct MapPosition **)malloc(map->height * sizeof(struct MapPosition *));

        for (int y = 0; y < map->height; y++) {
            map->grid[y] = (struct MapPosition *)calloc(map->width, sizeof(struct MapPosition));

            for (int x = 0; x < map->width; x++) {
                c = fgetc(inputFile);

                map->grid[y][x].type = PATH;

                switch (c) {
                case '#':
                    map->grid[y][x].type = OBSTRUCTION;
                    break;
                case '^':
                    map->direction = UP;
                    map->position.x = x;
                    map->position.y = y;
                    break;
                case 'v':
                    map->direction = DOWN;
                    map->position.x = x;
                    map->position.y = y;
                    break;
                case '<':
                    map->direction = LEFT;
                    map->position.x = x;
                    map->position.y = y;
                    break;
                case '>':
                    map->direction = RIGHT;
                    map->position.x = x;
                    map->position.y = y;
                    break;
                default:
                    break;
                }
            }

            fgetc(inputFile); // \n
        }

        map->originalPosition = map->position;
        map->originalDirection = map->direction;

        fclose(inputFile);
    }

    return map;
}

void freeMap(struct Map *map) {
    for (int y = 0; y < map->height; y++) {
        free(map->grid[y]);
    }

    free(map->grid);
    free(map);
}
