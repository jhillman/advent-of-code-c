#include <stdlib.h>
#include <stdbool.h>

struct XY {
    int x;
    int y;
};

enum Type {
    EMPTY,
    BOX,
    BOX_LEFT,
    BOX_RIGHT,
    WALL,
    ROBOT
};

struct Warehouse {
    int width;
    int height;
    enum Type **floor;
    struct XY robot;
};

void printWarehouse(struct Warehouse * warehouse) {
    for (int y = 0; y < warehouse->height; y++) {
        for (int x = 0; x < warehouse->width; x++) {
            switch (warehouse->floor[y][x]) {
            case ROBOT:
                printf("@");
                break;
            case EMPTY:
                printf(".");
                break;
            case BOX:
                printf("O");
                break;
            case BOX_LEFT:
                printf("[");
                break;
            case BOX_RIGHT:
                printf("]");
                break;
            case WALL:
                printf("#");
                break;
            }
        }

        printf("\n");
    }

    printf("\n");
}

bool canMove(struct Warehouse *warehouse, struct XY position, struct XY direction) {
    struct XY move = { position.x + direction.x, position.y + direction.y };
    enum Type type = warehouse->floor[move.y][move.x];
    bool can = false;

    if (type == EMPTY) {
        can = true;
    } else if (type == BOX) {
        can = canMove(warehouse, move, direction);
    } else if (type == BOX_LEFT) {
        can = canMove(warehouse, move, direction);
        
        if (can && direction.y) {
             can = canMove(warehouse, (struct XY){ move.x + 1, move.y }, direction);
        }
    } else if (type == BOX_RIGHT) {
        can = canMove(warehouse, move, direction);
        
        if (can && direction.y) {
             can = canMove(warehouse, (struct XY){ move.x - 1, move.y }, direction);
        }
    }

    return can;
}

void performMove(struct Warehouse *warehouse, struct XY position, struct XY direction) {
    struct XY move = { position.x + direction.x, position.y + direction.y };
    enum Type type = warehouse->floor[move.y][move.x];

    if (type == BOX) {
        performMove(warehouse, move, direction);
    } else if (type == BOX_LEFT) {
        performMove(warehouse, move, direction);

        if (direction.y) {
            performMove(warehouse, (struct XY){ move.x + 1, move.y }, direction);
        }
    } else if (type == BOX_RIGHT) {
        performMove(warehouse, move, direction);

        if (direction.y) {
            performMove(warehouse, (struct XY){ move.x - 1, move.y }, direction);
        }
    }

    warehouse->floor[move.y][move.x] = warehouse->floor[move.y + direction.y * -1][move.x + direction.x * -1];
    warehouse->floor[move.y + direction.y * -1][move.x + direction.x * -1] = EMPTY;
}

int boxGPSSum(char *input, bool wide) {
    FILE *inputFile = fopen(input, "r");
    int sum = 0;

    if (inputFile) {
        struct Warehouse *warehouse = (struct Warehouse *)calloc(1, sizeof(struct Warehouse));
        struct XY direction;

        while (fgetc(inputFile) != '\n') {
            ++warehouse->width;
            ++warehouse->height;
        }

        if (wide) {
            warehouse->width *= 2;
        }

        warehouse->floor = (enum Type **)malloc(warehouse->height * sizeof(enum Type *));

        for (int y = 0; y < warehouse->height; y++) {
            warehouse->floor[y] = (enum Type *)malloc(warehouse->width * sizeof(enum Type));

            for (int x = 0; x < warehouse->width; x++) {
                if (y == 0) {
                    warehouse->floor[y][x] = WALL;
                } else {
                    switch (fgetc(inputFile)) {
                    case '@':
                        warehouse->floor[y][x] = ROBOT;
                        warehouse->robot.y = y;
                        warehouse->robot.x = x;

                        if (wide) {
                            warehouse->floor[y][++x] = EMPTY;
                        }
                        break;
                    case '.':
                        warehouse->floor[y][x] = EMPTY;

                        if (wide) {
                            warehouse->floor[y][++x] = EMPTY;
                        }
                        break;
                    case 'O':
                        if (wide) {
                            warehouse->floor[y][x++] = BOX_LEFT;
                            warehouse->floor[y][x] = BOX_RIGHT;
                        } else {
                            warehouse->floor[y][x] = BOX;
                        }
                        break;
                    case '#':
                        warehouse->floor[y][x] = WALL;

                        if (wide) {
                            warehouse->floor[y][++x] = WALL;
                        }
                        break;
                    }
                }
            }

            if (y > 0) {
                fgetc(inputFile); // \n
            }
        }

        while (!feof(inputFile)) {
            switch (fgetc(inputFile)) {
            case '^':
                direction.y = -1;
                direction.x = 0;
                break;
            case 'v':
                direction.y = 1;
                direction.x = 0;
                break;
            case '<':
                direction.y = 0;
                direction.x = -1;
                break;
            case '>':
                direction.y = 0;
                direction.x = 1;
                break;
            default: 
                continue;
            }

            if (canMove(warehouse, warehouse->robot, direction)) {
                performMove(warehouse, warehouse->robot, direction);
                
                warehouse->robot.x += direction.x;
                warehouse->robot.y += direction.y;
            }
        }

        fclose(inputFile);

        for (int y = 0; y < warehouse->height; y++) {
            for (int x = 0; x < warehouse->width; x++) {
                if (warehouse->floor[y][x] == BOX || warehouse->floor[y][x] == BOX_LEFT) {
                    sum += 100 * y + x;
                }
            }
        }

        for (int y = 0; y < warehouse->height; y++) {
            free(warehouse->floor[y]);
        }

        free(warehouse->floor);
        free(warehouse);
    }

    return sum;
}
