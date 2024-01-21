#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

enum Direction {
    NONE  = 0,
    UP    = 1 << 0,
    DOWN  = 1 << 2,
    LEFT  = 1 << 3,
    RIGHT = 1 << 4
};

struct Beam {
    int x;
    int y;
    enum Direction direction;
    bool active;
};

struct Beams {
    int capacity;
    int count;
    struct Beam *data;
};

struct Tile {
    char type;
    bool energized;
    enum Direction direction;
};

struct Contraption {
    int width;
    int height;
    struct Tile **tiles;
    struct Beams *beams;
};

void freeBeams(struct Beams *beams) {
    free(beams->data);
    free(beams);
}

void freeContraption(struct Contraption *contraption) {
    freeBeams(contraption->beams);

    for (int y = 0; y < contraption->height; y++) {
        free(contraption->tiles[y]);
    }

    free(contraption->tiles);
    free(contraption);
}

void addBeam(struct Beams *beams, struct Beam beam) {
    if (beams->count == beams->capacity) {
        beams->capacity += 50;
        beams->data = (struct Beam *)realloc(beams->data, beams->capacity * sizeof(struct Beam));
    }

    beams->data[beams->count++] = beam;
}

void updateBeam(struct Contraption *contraption, struct Beam *beam) {
    switch (beam->direction) {
    case UP:
        --beam->y;
        break;
    case DOWN:
        ++beam->y;
        break;
    case LEFT:
        --beam->x;
        break;
    case RIGHT:
        ++beam->x;
        break;
    default:
        break;
    }

    beam->active = beam->x >= 0 && beam->x < contraption->width && 
                   beam->y >= 0 && beam->y < contraption->height;

    if (beam->active && contraption->tiles[beam->y][beam->x].direction & beam->direction) {
        beam->active = false;
    }
}

struct Contraption *getContraption(char *input) {
    FILE *inputFile = fopen(input, "r");
    struct Contraption *contraption = NULL;

    if (inputFile) {
        char c;

        contraption = (struct Contraption *)calloc(1, sizeof(struct Contraption));
        contraption->beams = (struct Beams *)calloc(1, sizeof(struct Beams));

        while (!feof(inputFile)) {
            c = fgetc(inputFile);

            if (c == '\n' || feof(inputFile)) {
                ++contraption->height;
            }

            if (!contraption->height) {
                ++contraption->width;
            }
        }

        fseek(inputFile, 0, SEEK_SET);

        contraption->tiles = (struct Tile **)malloc(contraption->height * sizeof(struct Tile *));

        for (int y = 0; y < contraption->height; y++) {
            contraption->tiles[y] = (struct Tile *)calloc(contraption->width, sizeof(struct Tile));

            for (int x = 0; x < contraption->width; x++) {
                contraption->tiles[y][x].type = fgetc(inputFile);
            }

            fgetc(inputFile); // \n
        }

        fclose(inputFile);
    }

    return contraption;
}

int energizedTiles(struct Contraption *contraption, struct Beams *beams) {
    bool beamsActive;
    int energized;
    int energizedMax = 0;

    for (int i = 0; i < beams->count; i++) {
        contraption->beams->count = 0;
        beamsActive = true;

        addBeam(contraption->beams, beams->data[i]);

        while (beamsActive) {
            beamsActive = false;

            for (int i = 0; i < contraption->beams->count; i++) {
                struct Beam *beam = &contraption->beams->data[i];

                if (beam->active) {
                    contraption->tiles[beam->y][beam->x].energized = true;
                    contraption->tiles[beam->y][beam->x].direction |= beam->direction;

                    switch (contraption->tiles[beam->y][beam->x].type) {
                    case '/':
                        switch (beam->direction) {
                        case UP:
                            beam->direction = RIGHT;
                            break;
                        case DOWN:
                            beam->direction = LEFT;
                            break;
                        case LEFT:
                            beam->direction = DOWN;
                            break;
                        case RIGHT:
                            beam->direction = UP;
                            break;
                        default:
                            break;
                        }
                        break;
                    case '\\':
                        switch (beam->direction) {
                        case UP:
                            beam->direction = LEFT;
                            break;
                        case DOWN:
                            beam->direction = RIGHT;
                            break;
                        case LEFT:
                            beam->direction = UP;
                            break;
                        case RIGHT:
                            beam->direction = DOWN;
                            break;
                        default:
                            break; 
                        }
                        break;
                    case '-':
                        if (beam->direction == UP || beam->direction == DOWN) {
                            struct Beam newBeam = { beam->x, beam->y, LEFT, true };
                            updateBeam(contraption, &newBeam);

                            if (newBeam.active) {
                                addBeam(contraption->beams, newBeam);
                            }

                            beam->direction = RIGHT;
                        }
                        break;
                    case '|':
                        if (beam->direction == LEFT || beam->direction == RIGHT) {
                            struct Beam newBeam = { beam->x, beam->y, UP, true };
                            updateBeam(contraption, &newBeam);

                            if (newBeam.active) {
                                addBeam(contraption->beams, newBeam);
                            }

                            beam->direction = DOWN;
                        }
                        break;
                    }

                    updateBeam(contraption, beam);
                }

                beamsActive = beamsActive || beam->active;
            }
        }

        energized = 0;

        for (int y = 0; y < contraption->height; y++) {
            for (int x = 0; x < contraption->width; x++) {
                energized += contraption->tiles[y][x].energized ? 1 : 0;
                contraption->tiles[y][x].energized = false;
                contraption->tiles[y][x].direction = NONE;
            }
        }

        if (energized > energizedMax) {
            energizedMax = energized;
        }            
    }

    return energizedMax;
}