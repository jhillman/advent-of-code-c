#include "../intcode.h"
#include <math.h>

enum Direction {
    UP    = '^',
    DOWN  = 'v',
    LEFT  = '<',
    RIGHT = '>'
};

struct Location {
    int x;
    int y;
};

struct Robot {
    struct Location location;
    enum Direction direction;
};

enum Command {
    NONE,
    FORWARD,
    TURN_LEFT,
    TURN_RIGHT
};

struct Scaffold {
    int width;
    int height;
    char **grid;
    
    struct Program *program;
    struct Robot robot;
};

struct Location updatedLocation(struct Robot robot, enum Command command) {
    struct Location updated = robot.location;

    switch (command) {
    case FORWARD:
        switch (robot.direction) {
        case UP:
            --updated.y;
            break;
        case DOWN:
            ++updated.y;
            break;
        case LEFT:
            --updated.x;
            break;
        case RIGHT:
            ++updated.x;
            break;
        }
        break;
    case TURN_LEFT:
        switch (robot.direction) {
        case UP:
            --updated.x;
            break;
        case DOWN:
            ++updated.x;
            break;
        case LEFT:
            ++updated.y;
            break;
        case RIGHT:
            --updated.y;
            break;
        }
        break;
    case TURN_RIGHT:
        switch (robot.direction) {
        case UP:
            ++updated.x;
            break;
        case DOWN:
            --updated.x;
            break;
        case LEFT:
            --updated.y;
            break;
        case RIGHT:
            ++updated.y;
            break;
        }
        break;
    default:
        break;
    }

    return updated;
}

bool locationValid(struct Scaffold *scaffold, struct Location location) {
    if (!(location.x >= 0 && location.x < scaffold->width && location.y >= 0 && location.y < scaffold->height)) {
        return false;
    }

    switch (scaffold->grid[location.y][location.x]) {
    case '#':
    case '^':
    case 'v':
    case '<':
    case '>':
        return true;
    default:
        return false;
    }
}

enum Direction updatedDirection(enum Direction direction, enum Command command) {
    switch (command) {
    case FORWARD:
        return direction;
    case TURN_LEFT:
        switch (direction) {
        case UP:
            return LEFT;
        case DOWN:
            return RIGHT;
        case LEFT:
            return DOWN;
        case RIGHT:
            return UP;
        }
    case TURN_RIGHT:
        switch (direction) {
        case UP:
            return RIGHT;
        case DOWN:
            return LEFT;
        case LEFT:
            return UP;
        case RIGHT:
            return DOWN;
        }
    default:
        return direction;
    }
}

void printScaffold(struct Scaffold *scaffold) {
    for (int y = 0; y < scaffold->height; y++) {
        printf("%s", scaffold->grid[y]);
    }
}

struct Scaffold *getScaffold(char *input) {
    struct Program *program = loadProgram(input);
    struct Scaffold *scaffold = NULL;

    if (program) {
        scaffold = (struct Scaffold *)calloc(1, sizeof(struct Scaffold));
        scaffold->program = program;
        char character;
        char lastCharacter;

        pipe(scaffold->program->output);

        if (fork() == 0) {
            runProgram(scaffold->program);

            exit(0);
        }

        while (read(scaffold->program->output[READ], &character, sizeof(character))) {
            if (character) {
                if (!scaffold->height) {
                    ++scaffold->width;
                }

                if (character == '\n') {
                    if (lastCharacter == '\n') {
                        break;
                    }

                    ++scaffold->height;
                }

                lastCharacter = character;
            }
        }

        pipe(scaffold->program->output);

        if (fork() == 0) {
            runProgram(scaffold->program);

            exit(0);
        }

        scaffold->grid = (char **)calloc(scaffold->height, sizeof(char *));

        int x = 0;
        int y = 0;

        while (read(scaffold->program->output[READ], &character, sizeof(character))) {
            if (character) {
                switch (character) {
                case '^':
                case 'v':
                case '<':
                case '>':
                    scaffold->robot.location = (struct Location){ x, y };
                    scaffold->robot.direction = character;
                    break;
                }

                if (character == '\n' && lastCharacter == '\n') {
                    break;
                }

                if (x == 0) {
                    scaffold->grid[y] = (char *)malloc((scaffold->width + 1) * sizeof(char));
                }

                scaffold->grid[y][x++] = character;

                if (character == '\n') {
                    scaffold->grid[y++][x] = '\0';
                    x = 0;
                }

                lastCharacter = character;
            }
        }
    }

    return scaffold;
}

void freeScaffold(struct Scaffold *scaffold) {    
    freeProgram(scaffold->program);

    for (int y = 0; y < scaffold->height; y++) {
        free(scaffold->grid[y]);
    }

    free(scaffold->grid);
    free(scaffold);
}