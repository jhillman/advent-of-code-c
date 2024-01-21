#include <stdlib.h>
#include <stdbool.h>

enum Direction {
    EAST,
    SOUTHEAST,
    SOUTHWEST,
    WEST,
    NORTHEAST,
    NORTHWEST
};

enum Color {
    WHITE,
    BLACK
};

struct Tile {
    enum Color color;

    struct Tile *east;
    struct Tile *southeast;
    struct Tile *southwest;
    struct Tile *west;
    struct Tile *northeast;
    struct Tile *northwest;

    bool flagged;
};

void connectSurroundingTiles(struct Tile *tile) {
    if (tile) {
        if (tile->east && tile->northeast) {
            tile->east->northwest = tile->northeast;
            tile->northeast->southeast = tile->east;
        }

        if (tile->east && tile->southeast) {
            tile->east->southwest = tile->southeast;
            tile->southeast->northeast = tile->east;
        }
        
        if (tile->southeast && tile->southwest) {
            tile->southeast->west = tile->southwest;
            tile->southwest->east = tile->southeast;
        }
        
        if (tile->southwest && tile->west) {
            tile->southwest->northwest = tile->west;
            tile->west->southeast = tile->southwest;
        }
        
        if (tile->west && tile->northwest) {
            tile->west->northeast = tile->northwest;
            tile->northwest->southwest = tile->west;
        }
        
        if (tile->northwest && tile->northeast) {
            tile->northwest->east = tile->northeast;
            tile->northeast->west = tile->northwest;
        }
    }
}

void initializeTile(struct Tile *tile) {
    if (!tile->east) {
        tile->east = (struct Tile *) calloc(1, sizeof(struct Tile));
        tile->east->west = tile;
    } else {
        connectSurroundingTiles(tile->east);
    }

    if (!tile->southeast) {
        tile->southeast = (struct Tile *) calloc(1, sizeof(struct Tile));
        tile->southeast->northwest = tile;
    } else {
        connectSurroundingTiles(tile->southeast);
    }

    if (!tile->southwest) {
        tile->southwest = (struct Tile *) calloc(1, sizeof(struct Tile));
        tile->southwest->northeast = tile;
    } else {
        connectSurroundingTiles(tile->southwest);
    }

    if (!tile->west) {
        tile->west = (struct Tile *) calloc(1, sizeof(struct Tile));
        tile->west->east = tile;
    } else {
        connectSurroundingTiles(tile->west);
    }

    if (!tile->northwest) {
        tile->northwest = (struct Tile *) calloc(1, sizeof(struct Tile));
        tile->northwest->southeast = tile;
    } else {
        connectSurroundingTiles(tile->northwest);
    }

    if (!tile->northeast) {
        tile->northeast = (struct Tile *) calloc(1, sizeof(struct Tile));
        tile->northeast->southwest = tile;
    } else {
        connectSurroundingTiles(tile->northeast);
    }

    connectSurroundingTiles(tile);
}

void flipTile(struct Tile *center, enum Direction *directions, int directionCount) {
    struct Tile *current = center;

    for (int i = 0; i < directionCount; i++) {
        switch (directions[i]) {
            case EAST:
                initializeTile(current->east);
                current = current->east;
                break;
            case SOUTHEAST:
                initializeTile(current->southeast);
                current = current->southeast;
                break;
            case SOUTHWEST:
                initializeTile(current->southwest);
                current = current->southwest;
                break;
            case WEST:
                initializeTile(current->west);
                current = current->west;
                break;
            case NORTHEAST:
                initializeTile(current->northwest);
                current = current->northeast;
                break;
            case NORTHWEST:
                initializeTile(current->northeast);
                current = current->northwest;
                break;
        }

        initializeTile(current);
    }

    if (current->color == WHITE) {
        current->color = BLACK;
    } else {
        current->color = WHITE;
    }
}

void clearFlags(struct Tile *tile) {
    if (tile && tile->flagged) {
        tile->flagged = false;

        clearFlags(tile->east);
        clearFlags(tile->southeast);
        clearFlags(tile->southwest);
        clearFlags(tile->west);
        clearFlags(tile->northeast);
        clearFlags(tile->northwest);
    }
}

int countBlackTiles(struct Tile *tile) {
    if (tile && !tile->flagged) {
        tile->flagged = true;

        int count = tile->color == BLACK ? 1 : 0;

        count += countBlackTiles(tile->east);
        count += countBlackTiles(tile->southeast);
        count += countBlackTiles(tile->southwest);
        count += countBlackTiles(tile->west);
        count += countBlackTiles(tile->northwest);
        count += countBlackTiles(tile->northeast);

        return count;
    }

    return 0;
}

int countAllBlackTiles(struct Tile *tile) {
    int count = countBlackTiles(tile);

    clearFlags(tile);

    return count;
}

void freeTile(struct Tile *tile) {
    if (tile && !tile->flagged) {
        tile->flagged = true;

        freeTile(tile->east);
        freeTile(tile->southeast);
        freeTile(tile->southwest);
        freeTile(tile->west);
        freeTile(tile->northeast);
        freeTile(tile->northwest);

        free(tile);
    }
}

struct Tile *getTiles(char *input) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        struct Tile *center = (struct Tile *) calloc(1, sizeof(struct Tile));
        char *line = (char *) malloc(64 * sizeof(char));
        size_t length;
        char *charPtr;
        int blackTiles = 0;

        initializeTile(center);

        while (getline(&line, &length, inputFile) != -1) {
            charPtr = line;
            enum Direction *directions = NULL;
            int directionCount = 0;

            while (*charPtr && *charPtr != '\n') {
                if (directions) {
                    directions = (enum Direction *) realloc(directions, (directionCount + 1) * sizeof(enum Direction));
                } else {
                    directions = (enum Direction *) malloc(sizeof(enum Direction));
                }

                switch (*charPtr) {
                    case 'e':
                        directions[directionCount++] = EAST;
                        ++charPtr;
                        break;
                    case 'w':
                        directions[directionCount++] = WEST;
                        ++charPtr;
                        break;
                    case 's':
                        if (*++charPtr == 'e') {
                            directions[directionCount++] = SOUTHEAST;
                        } else {
                            directions[directionCount++] = SOUTHWEST;
                        }

                        ++charPtr;
                        break;
                    case 'n':
                        if (*++charPtr == 'e') {
                            directions[directionCount++] = NORTHEAST;
                        } else {
                            directions[directionCount++] = NORTHWEST;
                        }

                        ++charPtr;
                        break;
                }
            }

            flipTile(center, directions, directionCount);

            free(directions);
        }

        fclose(inputFile);
        free(line);

        return center;
    }

    return NULL;
}
