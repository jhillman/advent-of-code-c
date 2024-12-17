#include <stdlib.h>
#include <stdbool.h>

#define SPACE_WIDTH 101
#define SPACE_HEIGHT 103

struct XY {
    int x;
    int y;
};

struct Robot {
    struct XY position;
    struct XY velocity;
};

struct Robots {
    int capacity;
    int count;
    struct Robot *data;
};

struct Space {
    int **tiles;
    struct Robots *robots;

};

void addRobot(struct Robots *robots, struct Robot robot) {
    if (robots->count == robots->capacity) {
        robots->capacity += 100;
        robots->data = (struct Robot *)realloc(robots->data, robots->capacity * sizeof(struct Robot));
    }

    robots->data[robots->count++] = robot;
}

void moveRobots(struct Space *space) {
    for (int i = 0; i < space->robots->count; i++) {
        --space->tiles[space->robots->data[i].position.y][space->robots->data[i].position.x];

        space->robots->data[i].position.x += space->robots->data[i].velocity.x;
        space->robots->data[i].position.x %= SPACE_WIDTH;

        if (space->robots->data[i].position.x < 0) {
            space->robots->data[i].position.x += SPACE_WIDTH;
        }

        space->robots->data[i].position.y += space->robots->data[i].velocity.y;
        space->robots->data[i].position.y %= SPACE_HEIGHT;

        if (space->robots->data[i].position.y < 0) {
            space->robots->data[i].position.y += SPACE_HEIGHT;
        }

        ++space->tiles[space->robots->data[i].position.y][space->robots->data[i].position.x];
    }
}

struct Space *getSpace(char *input) {
    FILE *inputFile = fopen(input, "r");
    struct Space *space = NULL;

    if (inputFile) {
        space = (struct Space *)calloc(1, sizeof(struct Space));
        space->tiles = (int **)malloc(SPACE_HEIGHT * sizeof(int *));

        for (int y = 0; y < SPACE_HEIGHT; y++) {
            space->tiles[y] = (int *)calloc(SPACE_WIDTH, sizeof(int));
        }

        space->robots = (struct Robots *)calloc(1, sizeof(struct Robots));
        struct Robot robot;

        while (!feof(inputFile) && fscanf(inputFile, "p=%d,%d v=%d,%d ", 
            &robot.position.x, &robot.position.y, &robot.velocity.x, &robot.velocity.y)) {
            ++space->tiles[robot.position.y][robot.position.x];

            addRobot(space->robots, robot);
        }

        fclose(inputFile);
    }

    return space;
}

void freeSpace(struct Space *space) {
    for (int y = 0; y < SPACE_HEIGHT; y++) {
        free(space->tiles[y]);
    }

    free(space->robots->data);
    free(space->robots);
    free(space->tiles);
    free(space);
}
