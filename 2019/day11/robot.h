#include <stdbool.h>
#include "../intcode.h"

#define GRID_SIZE 200
#define ROBOT_HALT -1L
#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

enum Color {
    BLACK,
    WHITE
};

enum Turn {
    TURN_LEFT,
    TURN_RIGHT
};

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct Panel {
    enum Color color;
    bool painted;
};

struct Robot {
    struct Program *program;
    struct Panel **panels;
    int x;
    int y;
    int minX;
    int maxX;
    int minY;
    int maxY;
    enum Direction direction;
};

struct Robot *getRobot(char *input) {
    struct Robot *robot = (struct Robot *)calloc(1, sizeof(struct Robot));

    robot->program = loadProgram(input);
    robot->program->haltValue = ROBOT_HALT;
    robot->panels = (struct Panel **)calloc(GRID_SIZE, sizeof(struct Panel *));
    robot->x = GRID_SIZE / 2;
    robot->y = GRID_SIZE / 2;
    robot->minX = GRID_SIZE;
    robot->maxX = 0;
    robot->minY = GRID_SIZE;
    robot->maxY = 0;
    robot->direction = UP;

    for (int y = 0; y < GRID_SIZE; y++) {
        robot->panels[y] = (struct Panel *)calloc(GRID_SIZE, sizeof(struct Panel));
    }

    return robot;
}

void runRobot(struct Robot *robot, enum Color startingColor) {
    pipe(robot->program->input);
    pipe(robot->program->output);

    long colorInput = startingColor;

    write(robot->program->input[WRITE], &colorInput, sizeof(colorInput));

    if (fork() == 0) {
        runProgram(robot->program);

        exit(0);
    }

    long output;

    while (true) {
        read(robot->program->output[READ], &output, sizeof(output));

        if (output == ROBOT_HALT) {
            break;
        }

        enum Color color = output;

        read(robot->program->output[READ], &output, sizeof(output));

        if (output == ROBOT_HALT) {
            break;
        }

        enum Turn turn = output;

        robot->panels[robot->y][robot->x].color = color;
        robot->panels[robot->y][robot->x].painted = true;

        switch (turn) {
        case TURN_LEFT:
            switch(robot->direction) {
                case UP:
                    robot->direction = LEFT;
                    --robot->x;
                    break;
                case DOWN:
                    robot->direction = RIGHT;
                    ++robot->x;
                    break;
                case LEFT:
                    robot->direction = DOWN;
                    ++robot->y;
                    break;
                case RIGHT:
                    robot->direction = UP;
                    --robot->y;
                    break;
            }
            break;
        case TURN_RIGHT:
            switch(robot->direction) {
                case UP:
                    robot->direction = RIGHT;
                    ++robot->x;
                    break;
                case DOWN:
                    robot->direction = LEFT;
                    --robot->x;
                    break;
                case LEFT:
                    robot->direction = UP;
                    --robot->y;
                    break;
                case RIGHT:
                    robot->direction = DOWN;
                    ++robot->y;
                    break;
            }
        }

        robot->minX = min(robot->x, robot->minX);
        robot->maxX = max(robot->x, robot->maxX);
        robot->minY = min(robot->y, robot->minY);
        robot->maxY = max(robot->y, robot->maxY);

        colorInput = robot->panels[robot->y][robot->x].color;

        write(robot->program->input[WRITE], &colorInput, sizeof(colorInput));
    }

    wait(NULL);
}

void freeRobot(struct Robot *robot) {
    freeProgram(robot->program);

    for (int y = 0; y < GRID_SIZE; y++) {
        free(robot->panels[y]);
    }

    free(robot->panels);
    free(robot);
}
