#include "../../aoc.h"
#include <stdlib.h>
#include <stdbool.h>

#define MAX_STEPS 10000

enum Direction {
    NONE,
    UP,
    LEFT,
    RIGHT,
    DOWN
};

enum UnitType {
    ELF,
    GOBLIN
};

struct Location {
    int x;
    int y;
};

int compareLocations(struct Location one, struct Location two) {
    int result = one.y - two.y;

    if (result == 0) {
        result = one.x - two.x;
    }

    return result;
}

struct Unit {
    int index;
    enum UnitType type;
    
    struct Location original;
    struct Location location;

    int attackPower;
    int hitPoints;
    bool dead;
};

int compare(const void *a, const void *b) {
    struct Location locationA = (*(struct Unit **)a)->location;
    struct Location locationB = (*(struct Unit **)b)->location;

    return compareLocations(locationA, locationB);
}

struct Units {
    int capacity;
    int count;
    struct Unit *data;
    struct Unit **addresses;
};

struct Unit *addUnit(struct Units *units, struct Unit unit) {
    if (units->count == units->capacity) {
        units->capacity += 100;
        units->data = (struct Unit *)realloc(units->data, units->capacity * sizeof(struct Unit));
        units->addresses = (struct Unit **)realloc(units->addresses, units->capacity * sizeof(struct Unit *));
    }

    units->data[units->count] = unit;

    return &units->data[units->count++];
}

void freeUnits(struct Units *units) {
    free(units->data);
    free(units->addresses);
    free(units);
}

struct TargetPath {
    struct Location start;
    struct Location step;
    enum Direction direction;
    int steps;
};

struct TargetPath emptyTargetPath() {
    return (struct TargetPath){ { -1, -1, }, { -1, -1 }, NONE, 0 };
}

bool isEmpty(struct TargetPath path) {
    return path.direction == NONE && path.steps == 0;
}

struct TargetPaths {
    int capacity;
    int count;
    int index;
    struct TargetPath *data;
};

void addTargetPath(struct TargetPaths *paths, struct TargetPath path) {
    if (paths->count == paths->capacity) {
        paths->capacity += 100;
        paths->data = (struct TargetPath *)realloc(paths->data, paths->capacity * sizeof(struct TargetPath));
    }

    paths->data[paths->count++] = path;
}

void freeTargetPaths(struct TargetPaths *paths) {
    free(paths->data);
    free(paths);
}

struct TargetPathQueue {
    int size;
    int capacity;
    struct TargetPaths **data;
    int fewestSteps;
};

void enqueue(struct TargetPathQueue *queue, struct TargetPath path) {
    if (path.steps >= queue->capacity) {
        int capacity = queue->capacity;

        queue->capacity = path.steps + 100;
        queue->data = (struct TargetPaths **)realloc(queue->data, queue->capacity * sizeof(struct TargetPaths *));

        for (int i = capacity; i < queue->capacity; i++) {
            queue->data[i] = (struct TargetPaths *)calloc(1, sizeof(struct TargetPaths));
        }
    }

    addTargetPath(queue->data[path.steps], path);

    if (!queue->fewestSteps || path.steps < queue->fewestSteps) {
        queue->fewestSteps = path.steps;
    }

    ++queue->size;
}

struct TargetPath dequeue(struct TargetPathQueue *queue) {
    struct TargetPaths *paths = queue->data[queue->fewestSteps];
    struct TargetPath path = paths->data[paths->index++];

    --queue->size;
    
    if (paths->index == paths->count) {
        paths->index = 0;
        paths->count = 0;

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

    return path;
}

void clearTargetPathQueue(struct TargetPathQueue *queue) {
    for (int i = 0; i < queue->capacity; i++) {
        queue->data[i]->count = 0;
        queue->data[i]->index = 0;
    }

    queue->size = 0;
    queue->fewestSteps = 0;
}

void freeTargetPathQueue(struct TargetPathQueue *queue) {
    for (int i = 0; i < queue->capacity; i++) {
        freeTargetPaths(queue->data[i]);
    }

    free(queue->data);
    free(queue);
}

enum SquareType {
    WALL,
    OPEN,
    UNIT
};

struct Square {
    enum SquareType type;
    char unitIndex;
};

struct Battle {
    int width;
    int height;
    struct Square **map;

    struct Units *units;
    struct TargetPathQueue *queue;
    int ***cumulativeSteps;

    int rounds;
    bool ended;
    int outcome;
};

void freeBattle(struct Battle *battle) {
    for (int y = 0; y < battle->height; y++) {
        free(battle->map[y]);

        for (int x = 0; x < battle->width; x++) {
            if (battle->map[y][x].type != WALL) {
                free(battle->cumulativeSteps[y][x]);
            }
        }

        free(battle->cumulativeSteps[y]);
    }

    free(battle->map);
    free(battle->cumulativeSteps);

    freeUnits(battle->units);
    freeTargetPathQueue(battle->queue);
    
    free(battle);
}

void takeTurn(struct Battle *battle, struct Unit *unit, bool tryMoving) {
    enum Direction directions[] = { UP, LEFT, RIGHT, DOWN };
    int xDeltas[] = {0, -1, 1, 0};
    int yDeltas[] = {-1, 0, 0, 1};

    struct Unit *other;
    struct Unit *target = NULL;

    struct TargetPath targetPath;
    struct TargetPath path;
    struct Location step;
    enum Direction direction;

    for (int i = 0; i < 4; i++) {
        step = (struct Location){ unit->location.x + xDeltas[i], unit->location.y + yDeltas[i] };

        if (!(step.x >= 0 && step.x < battle->width && step.y >= 0 && step.y < battle->height)) {
            continue;
        }

        if (battle->map[step.y][step.x].type == UNIT) {
            other = &battle->units->data[battle->map[step.y][step.x].unitIndex];

            if (other->type == unit->type) {
                continue;
            }

            if (!target || other->hitPoints < target->hitPoints) {
                target = other;
            }
        }
    }

    if (target) {
        target->hitPoints -= unit->attackPower;

        if (target->hitPoints <= 0) {
            target->dead = true;
            battle->map[target->location.y][target->location.x] = (struct Square){ OPEN, -1 };
        }
    } else if (tryMoving) {
        targetPath = emptyTargetPath();

        for (int i = 0; i < battle->units->count; i++) {
            struct Unit *opponent = &battle->units->data[i];

            if (opponent->type == unit->type || opponent->dead) {
                continue;
            }

            for (int y = 0; y < battle->height; y++) {
                for (int x = 0; x < battle->width; x++) {
                    if (battle->map[y][x].type != WALL) {
                        for (int d = 0; d < 5; d++) {
                            battle->cumulativeSteps[y][x][d] = MAX_STEPS;
                        }
                    }
                }
            }

            battle->cumulativeSteps[unit->location.y][unit->location.x][NONE] = 0;

            clearTargetPathQueue(battle->queue);

            for (int j = 0; j < 4; j++) {
                step = (struct Location){ unit->location.x + xDeltas[j], unit->location.y + yDeltas[j] };
                direction = directions[j];

                if (!(step.x >= 0 && step.x < battle->width && step.y >= 0 && step.y < battle->height)) {
                    continue;
                }

                if (battle->map[step.y][step.x].type != OPEN) {
                    continue;
                }

                enqueue(battle->queue, (struct TargetPath) {
                    step, 
                    step, 
                    direction, 
                    1
                });
            }

            while (battle->queue->size) {
                path = dequeue(battle->queue);

                if (compareLocations(path.step, opponent->location) == 0) {
                    if (isEmpty(targetPath)) {
                        targetPath = path;
                    } else if (path.steps < targetPath.steps) {
                        targetPath = path;
                    } else if (path.steps == targetPath.steps && compareLocations(path.start, targetPath.start) < 0) {
                        targetPath = path;
                    }

                    break;
                }

                if (!isEmpty(targetPath) && path.steps > targetPath.steps) {
                    continue;
                }

                for (int j = 0; j < 4; j++) {
                    step = (struct Location){ path.step.x + xDeltas[j], path.step.y + yDeltas[j] };
                    direction = directions[j];

                    if (battle->map[step.y][step.x].type == WALL) {
                        continue;
                    }

                    if (battle->map[step.y][step.x].type == UNIT && battle->map[step.y][step.x].unitIndex != opponent->index) {
                        continue;
                    }

                    int *steps = &battle->cumulativeSteps[step.y][step.x][direction];

                    if (*steps > path.steps + 1) {
                        *steps = path.steps + 1;

                        enqueue(battle->queue, (struct TargetPath){ 
                            path.start, 
                            step, 
                            direction, 
                            path.steps + 1 
                        });
                    }
                }
            }
        }

        if (!isEmpty(targetPath)) {
            battle->map[targetPath.start.y][targetPath.start.x] = (struct Square){ UNIT, unit->index };
            battle->map[unit->location.y][unit->location.x] = (struct Square){ OPEN, -1 };
            unit->location = targetPath.start;

            takeTurn(battle, unit, false);
        }
    }
}

void printBattle(struct Battle *battle) {
    struct Unit *unit;

    for (int y = 0; y < battle->height; y++) {
        for (int x = 0; x < battle->width; x++) {
            switch (battle->map[y][x].type) {
            case WALL:
                printf("#");
                break;
            case OPEN:
                printf(".");
                break;
            case UNIT:
                printf("%c", battle->units->data[battle->map[y][x].unitIndex].type == ELF ? 'E' : 'G');
                break;
            }
        }

        if (y < battle->units->count) {
            unit = &battle->units->data[y];

            printf("  %02d %s - location: %02d, %02d, hitPoints: %d", y, (unit->type == ELF ? "elf   " : "goblin"), unit->location.y, unit->location.x, unit->hitPoints);
        }

        printf("\n");
    }

    printf("\n");
}

void evaluateBattle(struct Battle *battle) {
    int remainingElfCount = 0;
    bool remainingGoblinCount = 0;
    struct Unit *unit;

    for (int i = 0; (remainingElfCount == 0 || remainingGoblinCount == 0) && i < battle->units->count; i++) {
        unit = &battle->units->data[i];

        remainingElfCount += (unit->type == ELF && !unit->dead) ? 1 : 0;
        remainingGoblinCount += (unit->type == GOBLIN && !unit->dead) ? 1 : 0;
    }

    battle->ended = remainingElfCount == 0 || remainingGoblinCount == 0;
}

void proceed(struct Battle *battle) {
    struct Unit *unit;
    bool completedRound = false;

    qsort(battle->units->addresses, battle->units->count, sizeof(struct Unit *), compare);

    for (int i = 0; !battle->ended && i < battle->units->count; i++) {
        completedRound = (i == battle->units->count - 1);

        unit = battle->units->addresses[i];

        if (unit->dead) {
            continue;
        }

        takeTurn(battle, unit, true);

        evaluateBattle(battle);
    }

    if (completedRound) {
        ++battle->rounds;
    }

    if (battle->ended) {
        for (int i = 0; i < battle->units->count; i++) {
            unit = &battle->units->data[i];

            if (!unit->dead) {
                battle->outcome += unit->hitPoints;
            }
        }

        battle->outcome *= battle->rounds;
    }
}

struct Battle *getBattle(char *input) {
    FILE *inputFile = fopen(input, "r");
    
    struct Battle *battle = NULL;

    if (inputFile) {
        char c;
        struct Unit *unit;

        battle = (struct Battle *)calloc(1, sizeof(struct Battle));
        battle->units = (struct Units *)calloc(1, sizeof(struct Units));
        battle->queue = (struct TargetPathQueue *)calloc(1, sizeof(struct TargetPathQueue));

        while (!feof(inputFile)) {
            c = fgetc(inputFile);

            if (c == '\n' || feof(inputFile)) {
                ++battle->height;
            }

            if (!battle->height) {
                ++battle->width;
            }
        }

        fseek(inputFile, 0, SEEK_SET);

        battle->map = (struct Square **)malloc(battle->height * sizeof(struct Square *));
        battle->cumulativeSteps = (int ***)malloc(battle->height * sizeof(int **));

        for (int y = 0; y < battle->height; y++) {
            battle->map[y] = (struct Square *)malloc(battle->width * sizeof(struct Square));
            battle->cumulativeSteps[y] = (int **)malloc(battle->width * sizeof(int *));

            for (int x = 0; x < battle->width; x++) {
                c = fgetc(inputFile);

                if (c == 'E' || c == 'G') {
                    unit = addUnit(battle->units, (struct Unit){ battle->units->count, c == 'E' ? ELF : GOBLIN, { x, y }, { x, y }, 3, 200 });

                    battle->map[y][x] = (struct Square){ UNIT, unit->index };
                } else {
                    battle->map[y][x] = (struct Square){ c == '#' ? WALL : OPEN, -1 };
                }

                if (battle->map[y][x].type != WALL) {
                    battle->cumulativeSteps[y][x] = (int *)malloc(5 * sizeof(int));

                    for (int d = 0; d < 5; d++) {
                        battle->cumulativeSteps[y][x][d] = MAX_STEPS;
                    }
                }
            }

            fgetc(inputFile); // \n
        }

        fclose(inputFile);
    }

    for (int i = 0; i < battle->units->count; i++) {
        battle->units->addresses[i] = &battle->units->data[i];
    }

    return battle;
}

void resetBattle(struct Battle *battle) {
    struct Unit *unit;

    for (int i = 0; i < battle->units->count; i++) {
        unit = &battle->units->data[i];

        battle->map[unit->location.y][unit->location.x] = (struct Square){ OPEN, -1 };

        battle->units->data[i].location = battle->units->data[i].original;

        battle->units->data[i].hitPoints = 200;
        battle->units->data[i].dead = false;
    }

    for (int i = 0; i < battle->units->count; i++) {
        unit = &battle->units->data[i];

        battle->map[unit->location.y][unit->location.x] = (struct Square){ UNIT, unit->index };
    }

    battle->rounds = 0;
    battle->ended = false;
    battle->outcome = 0;
}

void setElfAttackPower(struct Battle *battle, int power) {
    struct Unit *unit;

    for (int i = 0; i < battle->units->count; i++) {
        unit = &battle->units->data[i];

        if (unit->type == ELF) {
            unit->attackPower = power;
        }
    }
}

bool allElvesSurvived(struct Battle *battle) {
    struct Unit *unit;
    bool allSurvived = true;

    for (int i = 0; allSurvived && i < battle->units->count; i++) {
        unit = &battle->units->data[i];

        if (unit->type == ELF) {
            allSurvived = !unit->dead;
        }
    }

    return allSurvived;
}