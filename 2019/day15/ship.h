#include "../intcode.h"
#include <string.h>

enum DroidStatus {
    HIT_WALL            =  0,
    MOVED               =  1,
    FOUND_OXYGEN_SYSTEM =  2
};

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
    case EAST:
        return WEST;
    case WEST:
        return EAST;
    default:
        return direction;
    }
}

struct Path {
    int capacity;
    int length;
    enum Direction *steps;
};

struct Location {
    int x;
    int y;
};

struct Location update(struct Location location, enum Direction direction) {
    switch (direction) {
    case NORTH:
        --location.y;
        break;
    case SOUTH:
        ++location.y;
        break;
    case EAST:
        ++location.x;
        break;
    case WEST:
        --location.x;
        break;
    default:
        break;
    }

    return location;
}

struct Droid {
    struct Location location;
    enum Direction direction;

    struct Path *path;
    int steps;
};

enum LocationType {
    UNKNOWN,
    WALL,
    PATH,
    OXYGEN_SYSTEM,
    OXYGEN
};

enum StepType {
    FORWARD,
    BACKWARD
};

struct Ship {
    int areaWidth;
    int areaHeight;
    enum LocationType **area;

    struct Program *program;
    struct Droid *droid;
    
    struct Location droidStart;
    struct Location oxygenSystem;
};

void updateShipArea(struct Ship *ship, struct Location *location) {
    bool expandNorth = location->y == -1;
    bool expandSouth = location->y == ship->areaHeight;
    bool expandWest = location->x == -1;
    bool expandEast = location->x == ship->areaWidth;

    if (expandNorth || expandSouth) {
        ship->areaHeight += ((expandNorth ? 1 : 0) + (expandSouth ? 1 : 0));

        ship->area = (enum LocationType **)realloc(ship->area, ship->areaHeight * sizeof(enum LocationType *));

        if (expandNorth) {
            memcpy(ship->area + 1, ship->area, (ship->areaHeight - 1) * sizeof(enum LocationType *));
            *ship->area = (enum LocationType *)malloc(ship->areaWidth * sizeof(enum LocationType));

            memset(*ship->area, UNKNOWN, ship->areaWidth * sizeof(enum LocationType));
        }

        if (expandSouth) {
            ship->area[ship->areaHeight - 1] = (enum LocationType *)malloc(ship->areaWidth * sizeof(enum LocationType));

            memset(ship->area[ship->areaHeight - 1], UNKNOWN, ship->areaWidth * sizeof(enum LocationType));
        }
    }

    if (expandWest || expandEast) {
        ship->areaWidth += ((expandWest ? 1 : 0) + (expandEast ? 1 : 0));

        for (int y = 0; y < ship->areaHeight; y++) {
            ship->area[y] = (enum LocationType *)realloc(ship->area[y], ship->areaWidth * sizeof(enum LocationType));

            if (expandWest) {
                memcpy(ship->area[y] + 1, ship->area[y], (ship->areaWidth - 1) * sizeof(enum LocationType));
                *ship->area[y] = UNKNOWN;
            }

            if (expandEast) {
                ship->area[y][ship->areaWidth - 1] = UNKNOWN;
            }
        }
    }

    if (expandNorth) {
        ++location->y;
        ++ship->droidStart.y;

        if (location != &ship->droid->location) {
            ++ship->droid->location.y;
        }
    }

    if (expandWest) {
        ++location->x;
        ++ship->droidStart.x;

        if (location != &ship->droid->location) {
            ++ship->droid->location.x;
        }
    }
}

void moveDroid(struct Droid *droid, enum StepType type) {
    if (droid->path->length == droid->path->capacity) {
        droid->path->capacity += 10;
        droid->path->steps = (enum Direction *)realloc(droid->path->steps, droid->path->capacity * sizeof(enum Direction));
    }

    switch (type) {
    case FORWARD:
        droid->path->steps[droid->path->length++] = droid->direction;
        break;
    case BACKWARD:
        droid->direction = opposite(droid->path->steps[--droid->path->length]);
        break;
    }
}

struct Ship *getShip(char *input) {
    struct Program *program = loadProgram(input);
    struct Ship *ship = NULL;

    if (program) {
        ship = (struct Ship *)calloc(1, sizeof(struct Ship));
        ship->areaWidth = 1;
        ship->areaHeight = 1;

        ship->area = (enum LocationType **)malloc(sizeof(enum LocationType *));
        *ship->area = (enum LocationType *)calloc(1, sizeof(enum LocationType));

        ship->program = program;
        ship->droid = (struct Droid *)calloc(1, sizeof(struct Droid));
    }

    return ship;
}

void findOxygenSystem(struct Ship *ship) {
    enum Direction directions[] = { NORTH, SOUTH, WEST, EAST };
    long input;
    long output;

    ship->droidStart = (struct Location){ 0, 0 };
    ship->droid->location = (struct Location){ 0, 0 };
    ship->droid->direction = NORTH;
    ship->droid->path = (struct Path *)calloc(1, sizeof(struct Path));

    pipe(ship->program->input);
    pipe(ship->program->output);

    input = ship->droid->direction;

    write(ship->program->input[WRITE], &input, sizeof(input));

    if (fork() == 0) {
        runProgram(ship->program);

        exit(0);
    }

    while (true) {
        read(ship->program->output[READ], &output, sizeof(output));

        enum LocationType type = UNKNOWN;

        switch (output) {
        case HIT_WALL:
            type = WALL;
            break;
        case MOVED:
            type = PATH;
            break;
        case FOUND_OXYGEN_SYSTEM:
            type = OXYGEN_SYSTEM;
            break;
        }

        if (output == HIT_WALL) {
            struct Location wall = update(ship->droid->location, ship->droid->direction);

            updateShipArea(ship, &wall);

            ship->area[wall.y][wall.x] = type;
        } else {
            ship->droid->location = update(ship->droid->location, ship->droid->direction);

            updateShipArea(ship, &ship->droid->location);

            if (ship->area[ship->droid->location.y][ship->droid->location.x] == UNKNOWN) {
                moveDroid(ship->droid, FORWARD);
            }

            ship->area[ship->droid->location.y][ship->droid->location.x] = type;
        }

        if (output == FOUND_OXYGEN_SYSTEM) {
            ship->oxygenSystem = ship->droid->location;
            ship->droid->steps = ship->droid->path->length;
        }

        enum Direction direction = NONE;

        for (int i = 0; direction == NONE && i < 4; i++) {
            struct Location step = update(ship->droid->location, directions[i]);

            updateShipArea(ship, &step);

            if (ship->area[step.y][step.x] == UNKNOWN) {
                direction = directions[i];
            }
        }

        if (direction == UNKNOWN) {
            moveDroid(ship->droid, BACKWARD);

            if (ship->droid->path->length == 0) {
                break;
            }
        } else {
            ship->droid->direction = direction;
        }

        input = ship->droid->direction;

        write(ship->program->input[WRITE], &input, sizeof(input));
    }

    freeProgram(ship->program);
}

void freeShip(struct Ship *ship) {
    for (int y = 0; y < ship->areaHeight; y++) {
        free(ship->area[y]);
    }

    free(ship->area);

    free(ship->droid->path->steps);
    free(ship->droid->path);
    free(ship->droid);

    free(ship);
}