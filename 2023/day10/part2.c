/* Day 10, part 2 = 563 */

#include "../../aoc.h"
#include "tiles.h"
#include <stdbool.h>

enum InsideDirection {
    NONE,
    NORTH,
    NORTH_EAST,
    EAST,
    SOUTH_EAST,
    SOUTH,
    SOUTH_WEST,
    WEST,
    NORTH_WEST
};

bool containsDirection(enum InsideDirection direction, enum InsideDirection roughDirection) {
    switch (roughDirection) {
    case NORTH:
        switch (direction) {
        case NORTH:
        case NORTH_EAST:
        case NORTH_WEST:
            return true;
        default:
            return false;
        }
    case SOUTH:
        switch (direction) {
        case SOUTH:
        case SOUTH_EAST:
        case SOUTH_WEST:
            return true;
        default:
            return false;
        }
    case EAST:
        switch (direction) {
        case EAST:
        case NORTH_EAST:
        case SOUTH_EAST:
            return true;
        default:
            return false;
        }
    case WEST:
        switch (direction) {
        case WEST:
        case NORTH_WEST:
        case SOUTH_WEST:
            return true;
        default:
            return false;
        }
    default:
        return false;
    }
}

struct Positions {
    int capacity;
    int count;
    struct Position *data;
};

void addPosition(struct Positions *positions, struct Position position) {
    if (positions->count == positions->capacity) {
        positions->capacity += 50;
        positions->data = (struct Position *)realloc(positions->data, positions->capacity * sizeof(struct Position));
    }

    positions->data[positions->count++] = position;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Tiles *tiles = getTiles(aoc.input);

    if (tiles) {
        int answer = tiles->width * tiles->height;
        struct Positions *positions = (struct Positions *)calloc(1, sizeof(struct Positions));
        char **pipeTiles;
        enum InsideDirection **insides;
        char lastPipe;
        char pipe;
        enum InsideDirection lastInside;
        enum InsideDirection *inside = NULL;
        int xDeltas[] = { 1, 0, -1, 0 };
        int yDeltas[] = { 0, 1, 0, -1 };
        enum InsideDirection insideDeltas[] = { WEST, NORTH, EAST, SOUTH };

        do {
            addPosition(positions, tiles->stepper1.current);

            tiles->stepper1 = updateStepper(tiles, tiles->stepper1);
        } while (!(tiles->stepper1.current.x == tiles->stepper2.current.x && 
                   tiles->stepper1.current.y == tiles->stepper2.current.y));

        answer -= positions->count;

        pipeTiles = (char **)malloc(tiles->height * sizeof(char *));

        for (int y = 0; y < tiles->height; y++) {
            pipeTiles[y] = (char *)malloc(tiles->width * sizeof(char));

            memset(pipeTiles[y], '.', tiles->width);
        }

        for (int i = 0; i < positions->count; i++) {
            pipeTiles[positions->data[i].y][positions->data[i].x] = tiles->data[positions->data[i].y][positions->data[i].x];
        }

        for (int y = 0; y < tiles->height; y++) {
            int x = 0;

            while (x < tiles->width && pipeTiles[y][x] == '.') {
                --answer;
                pipeTiles[y][x++] = ' ';
            }

            x = tiles->width - 1;

            while (x >= 0 && pipeTiles[y][x] == '.') {
                --answer;
                pipeTiles[y][x--] = ' ';
            }
        }    

        for (int x = 0; x < tiles->width; x++) {
            int y = 0;

            while (y < tiles->height && (pipeTiles[y][x] == ' ' || pipeTiles[y][x] == '.')) {
                if (pipeTiles[y][x] == '.') {
                    --answer;
                }

                pipeTiles[y++][x] = ' ';
            }

            y = tiles->height - 1;

            while (y >= 0 && (pipeTiles[y][x] == ' ' || pipeTiles[y][x] == '.')) {
                if (pipeTiles[y][x] == '.') {
                    --answer;
                }
                
                pipeTiles[y--][x] = ' ';
            }
        }    

        for (int y = 0; !inside && y < tiles->height; y++) {
            for (int x = 0; !inside && x < tiles->width; x++) {
                if (pipeTiles[y][x] == 'F') {
                    for (int i = 0; i < positions->count; i++) {
                        if (positions->data[i].y == y && positions->data[i].x == x) {
                            tiles->stepper1.current = tiles->stepper2.current = (struct Position)positions->data[i];
                            tiles->stepper1.last = (struct Position)positions->data[i + (i > 0 ? -1 : 1)];
                            break;
                        }
                    }

                    insides = (enum InsideDirection **)malloc(tiles->height * sizeof(enum InsideDirection *));

                    for (int i = 0; i < tiles->height; i++) {
                        insides[i] = (enum InsideDirection *)calloc(tiles->width, sizeof(enum InsideDirection));
                    }

                    inside = &insides[y][x];
                    *inside = SOUTH_EAST;
                }
            }
        }

        do {
            tiles->stepper1 = updateStepper(tiles, tiles->stepper1);

            lastPipe = pipeTiles[tiles->stepper1.last.y][tiles->stepper1.last.x];
            pipe = pipeTiles[tiles->stepper1.current.y][tiles->stepper1.current.x];

            lastInside = insides[tiles->stepper1.last.y][tiles->stepper1.last.x];
            inside = &insides[tiles->stepper1.current.y][tiles->stepper1.current.x];

            switch (lastPipe) {
            case '|':
                switch (pipe) {
                case '|':
                    *inside = lastInside;
                    break;
                case 'L':
                case '7':
                    *inside = containsDirection(lastInside, EAST) ? NORTH_EAST : SOUTH_WEST;
                    break;
                case 'J':
                case 'F':
                    *inside = containsDirection(lastInside, EAST) ? SOUTH_EAST : NORTH_WEST;
                    break;
                }
                break;
            case '-':
                switch (pipe) {
                case '-':
                    *inside = lastInside;
                    break;
                case 'L':
                case '7':
                    *inside = containsDirection(lastInside, NORTH) ? NORTH_EAST : SOUTH_WEST;
                    break;
                case 'J':
                case 'F':
                    *inside = containsDirection(lastInside, NORTH) ? NORTH_WEST : SOUTH_EAST;
                    break;
                }
                break;
            case 'L':
                switch (pipe) {
                case '|':
                    *inside = containsDirection(lastInside, EAST) ? EAST : WEST;
                    break;
                case '-':
                    *inside = containsDirection(lastInside, NORTH) ? NORTH : SOUTH;
                    break;
                case '7':
                    *inside = lastInside;
                    break;
                case 'J':
                    *inside = containsDirection(lastInside, NORTH) ? NORTH_WEST : SOUTH_EAST;
                    break;
                case 'F':
                    *inside = containsDirection(lastInside, NORTH) ? SOUTH_EAST : NORTH_WEST;
                    break;
                }
                break;
            case 'J':
                switch (pipe) {
                case '|':
                    *inside = containsDirection(lastInside, EAST) ? EAST : WEST;
                    break;
                case '-':
                    *inside = containsDirection(lastInside, NORTH) ? NORTH : SOUTH;
                    break;
                case 'F':
                    *inside = lastInside;
                    break;
                case 'L':
                    *inside = containsDirection(lastInside, NORTH) ? NORTH_EAST : SOUTH_WEST;
                    break;
                case '7':
                    *inside = containsDirection(lastInside, NORTH) ? SOUTH_WEST : NORTH_EAST;
                    break;
                }
                break;
            case '7':
                switch (pipe) {
                case '|':
                    *inside = containsDirection(lastInside, EAST) ? EAST : WEST;
                    break;
                case '-':
                    *inside = containsDirection(lastInside, NORTH) ? NORTH : SOUTH;
                    break;
                case 'L':
                    *inside = lastInside;
                    break;
                case 'J':
                    *inside = containsDirection(lastInside, NORTH) ? SOUTH_EAST : NORTH_WEST;
                    break;
                case 'F':
                    *inside = containsDirection(lastInside, NORTH) ? NORTH_WEST : SOUTH_EAST;
                    break;
                }
                break;
            case 'F':
                switch (pipe) {
                case '|':
                    *inside = containsDirection(lastInside, EAST) ? EAST : WEST;
                    break;
                case '-':
                    *inside = containsDirection(lastInside, NORTH) ? NORTH : SOUTH;
                    break;
                case 'J':
                    *inside = lastInside;
                    break;
                case 'L':
                    *inside = containsDirection(lastInside, NORTH) ? SOUTH_WEST : NORTH_EAST;
                    break;
                case '7':
                    *inside = containsDirection(lastInside, NORTH) ? NORTH_EAST : SOUTH_WEST;
                    break;
                }
                break;
            }
        } while (!(tiles->stepper1.current.x == tiles->stepper2.current.x && 
                   tiles->stepper1.current.y == tiles->stepper2.current.y));

        for (int y = 0; y < tiles->height; y++) {
            for (int x = 0; x < tiles->width; x++) {
                if (pipeTiles[y][x] == '.') {
                    for (int i = 0; pipeTiles[y][x] != ' ' && i < 4; i++) {
                        int tileX = x + xDeltas[i];
                        int tileY = y + yDeltas[i];

                        if (pipeTiles[tileY][tileX] == ' ' || 
                            (insides[tileY][tileX] && !containsDirection(insides[tileY][tileX], insideDeltas[i]))) {
                            --answer;
                            pipeTiles[y][x] = ' ';
                        }
                    }
                }
            }
        }

        for (int y = 0; y < tiles->height; y++) {
            free(insides[y]);
        }

        free(insides);

        free(positions->data);
        free(positions);

        for (int y = 0; y < tiles->height; y++) {
            free(pipeTiles[y]);
        }

        free(pipeTiles);

        freeTiles(tiles);

        print(aoc, INT, &answer);
    }

    return 0;
}
