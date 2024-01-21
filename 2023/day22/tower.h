#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

struct Coordinates {
    int x;
    int y;
    int z;
};

struct CoordinatesList {
    int capacity;
    int count;
    struct Coordinates *data;
};

void addCoordinates(struct CoordinatesList *coordinatesList, struct Coordinates coordinates) {
    if (coordinatesList->count == coordinatesList->capacity) {
        coordinatesList->capacity += 10;
        coordinatesList->data = (struct Coordinates *)realloc(coordinatesList->data, coordinatesList->capacity * sizeof(struct Coordinates));
    }

    coordinatesList->data[coordinatesList->count++] = coordinates;
}

struct Bricks;

struct Brick {
    int index;
    struct Coordinates start;
    struct Coordinates end;
    
    struct CoordinatesList *coordinates;

    struct Bricks *supporting;
    struct Bricks *supportedBy;
};

struct Bricks {
    int capacity;
    int count;
    struct Brick *data;
};

struct Brick makeBrick(struct Coordinates start, struct Coordinates end) {
    struct Brick brick = { -1,
        start, end, 
        calloc(1, sizeof(struct CoordinatesList)),
        calloc(1, sizeof(struct Bricks)),
        calloc(1, sizeof(struct Bricks))
     };

    for (int x = start.x; x <= end.x; x++) {
        for (int y = start.y; y <= end.y; y++) {
            for (int z = start.z; z <= end.z; z++) {
                addCoordinates(brick.coordinates, (struct Coordinates){ x, y, z });
            }
        }
    }

    return brick;
}

void addBrick(struct Bricks *bricks, struct Brick brick) {
    if (bricks->count == bricks->capacity) {
        bricks->capacity += 10;
        bricks->data = (struct Brick *)realloc(bricks->data, bricks->capacity * sizeof(struct Brick));
    }

    bricks->data[bricks->count++] = brick;
}

int compareBricks(const void *a, const void *b) {
    return ((struct Brick *)a)->start.z - ((struct Brick *)b)->start.z; 
}

void changeBrickDepth(struct Brick *brick, int zDelta) {
    struct Coordinates coordinates;

    brick->start = (struct Coordinates){ brick->start.x, brick->start.y, brick->start.z + zDelta };
    brick->end = (struct Coordinates){ brick->end.x, brick->end.y, brick->end.z + zDelta };

    for (int i = 0; i < brick->coordinates->count; i++) {
        brick->coordinates->data[i].z += zDelta;
    }
}

struct Tower {
    int height;
    int width;
    int depth;

    bool ***data;

    struct Bricks *bricks;
};

void updateBrickInTower(struct Tower *tower, struct Brick *brick, bool there) {
    struct Coordinates coordinates;

    for (int i = 0; i < brick->coordinates->count; i++) {
        coordinates = brick->coordinates->data[i];

        tower->data[coordinates.y][coordinates.x][coordinates.z] = there;
    }
}

bool towerContainsBrick(struct Tower *tower, struct Brick *brick) {
    struct Coordinates coordinates;
    bool contains = false;

    for (int i = 0; !contains && i < brick->coordinates->count; i++) {
        coordinates = brick->coordinates->data[i];

        contains = tower->data[coordinates.y][coordinates.x][coordinates.z];
    }

    return contains;
}

void letBricksFall(struct Tower *tower) {
    struct Brick *brick;

    for (int i = 0; i < tower->bricks->count; i++) {
        brick = &tower->bricks->data[i];

        do {
            changeBrickDepth(brick, -1);
        } while (brick->start.z > 0 && !towerContainsBrick(tower, brick));

        changeBrickDepth(brick, 1);

        updateBrickInTower(tower, brick, true);
    }
}

struct Tower *getTower(char *input) {
    FILE *inputFile = fopen(input, "r");
    struct Tower *tower = NULL;

    if (inputFile) {
        struct Coordinates start;
        struct Coordinates end;
        struct Coordinates max = { 0, 0, 0 };
        struct Brick *brick;
        struct Brick *other;

        tower = (struct Tower *)calloc(1, sizeof(struct Tower));
        tower->bricks = (struct Bricks *)calloc(1, sizeof(struct Bricks));

        while (fscanf(inputFile, "%d,%d,%d~%d,%d,%d", &start.x, &start.y, &start.z, &end.x, &end.y, &end.z) == 6) {
            max.x = max(max.x, max(start.x, end.x));
            max.y = max(max.y, max(start.y, end.y));
            max.z = max(max.z, max(start.z, end.z));

            addBrick(tower->bricks, makeBrick(start, end));
        }

        fclose(inputFile);

        qsort(tower->bricks->data, tower->bricks->count, sizeof(struct Brick), compareBricks);

        tower->width = max.x + 1;
        tower->height = max.y + 1;
        tower->depth = max.z + 1;

        tower->data = (bool ***)malloc(tower->height * sizeof(bool **));

        for (int y = 0; y < tower->height; y++) {
            tower->data[y] = (bool **)malloc(tower->width * sizeof(bool *));

            for (int x = 0; x < tower->width; x++) {
                tower->data[y][x] = (bool *)calloc(tower->depth, sizeof(bool));
            }
        }

        letBricksFall(tower);

        qsort(tower->bricks->data, tower->bricks->count, sizeof(struct Brick), compareBricks);

        for (int i = 0; i < tower->bricks->count; i++) {
            brick = &tower->bricks->data[i];
            brick->index = i;

            for (int j = i + 1; j < tower->bricks->count; j++) {
                other = &tower->bricks->data[j];
                other->index = j;

                if (brick->end.z < other->start.z - 1) {
                    continue;
                } else if (brick->end.z == other->start.z - 1 && 
                          (max(brick->start.x, other->start.x) <= min(brick->end.x, other->end.x) && 
                           max(brick->start.y, other->start.y) <= min(brick->end.y, other->end.y))) {
                    addBrick(other->supportedBy, *brick);
                    addBrick(brick->supporting, *other);
                }
            }
        }
    }

    return tower;
}

void freeTower(struct Tower *tower) {
    if (tower->bricks) {
        for (int i = 0; i < tower->bricks->count; i++) {
            free(tower->bricks->data[i].coordinates->data);
            free(tower->bricks->data[i].coordinates);
            free(tower->bricks->data[i].supporting->data);
            free(tower->bricks->data[i].supporting);
            free(tower->bricks->data[i].supportedBy->data);
            free(tower->bricks->data[i].supportedBy);
        }

        free(tower->bricks->data);
        free(tower->bricks);
    }

    for (int y = 0; y < tower->height; y++) {
        for (int x = 0; x < tower->width; x++) {
            free(tower->data[y][x]);
        }

        free(tower->data[y]);
    }

    free(tower->data);
    free(tower);
}