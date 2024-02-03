/* Day 24, part 2 = 3697 */

#include "../../aoc.h"
#include <math.h>
#include "tiles.h"

struct Grid {
    int width;
    int height;
    enum Color **tiles;
};

void getTileBounds(struct Tile *tile, int x, int y, int *minX, int *maxX, int *minY, int *maxY) {
    if (tile && !tile->flagged) {
        tile->flagged = true;

        getTileBounds(tile->east, x + 2, y, minX, maxX, minY, maxY);
        getTileBounds(tile->southeast, x + 1, y + 1, minX, maxX, minY, maxY);
        getTileBounds(tile->southwest, x - 1, y + 1, minX, maxX, minY, maxY);
        getTileBounds(tile->west, x - 2, y, minX, maxX, minY, maxY);
        getTileBounds(tile->northeast, x + 1, y - 1, minX, maxX, minY, maxY);
        getTileBounds(tile->northwest, x - 1, y - 1, minX, maxX, minY, maxY);

        if (tile->color == BLACK) {
            if (x < *minX) {
                *minX = x;
            }

            if (x > *maxX) {
                *maxX = x;
            }

            if (y < *minY) {
                *minY = y;
            }

            if (y > *maxY) {
                *maxY = y;
            }
        }
    }
}

void setBlackTiles(struct Tile *tile, enum Color **tiles, int x, int y) {
    if (tile && !tile->flagged) {
        tile->flagged = true;

        setBlackTiles(tile->east, tiles, x + 2, y);
        setBlackTiles(tile->southeast, tiles, x + 1, y + 1);
        setBlackTiles(tile->southwest, tiles, x - 1, y + 1);
        setBlackTiles(tile->west, tiles, x - 2, y);
        setBlackTiles(tile->northeast, tiles, x + 1, y - 1);
        setBlackTiles(tile->northwest, tiles, x - 1, y - 1);

        if (tile->color == BLACK) {
            tiles[y][x] = BLACK;
        }
    }
}

void expandGrid(struct Grid *grid) {
    bool tileOnTop = false;
    bool tileOnBottom = false;
    bool tileOnLeft = false;
    bool tileOnRight = false;

    bool adjustForOdd = false;

    for (int y = 0; y < grid->height; y++) {
        if (grid->tiles[y][0] == BLACK || grid->tiles[y][1] == BLACK) {
            tileOnLeft = true;
        }

        if (grid->tiles[y][grid->width - 1] == BLACK || grid->tiles[y][grid->width - 2] == BLACK) {
            tileOnRight = true;
        }

        if (y == 0 || y == grid->height - 1) {
            for (int x = 0; x < grid->width; x++) {
                if (grid->tiles[y][x] == BLACK) {
                    if (y == 0) {
                        tileOnTop = true;

                        adjustForOdd = x % 2 == 0;
                    } else {
                        tileOnBottom = true;
                    }
                }
            }
        }
    }

    bool tilesOnEdges = false;

    if (tileOnTop) {
        tilesOnEdges = true;
        grid->height += adjustForOdd ? 2 : 1;
    }

    if (tileOnBottom) {
        tilesOnEdges = true;
        grid->height += 1;
    }

    if (tileOnLeft) {
        tilesOnEdges = true;
        grid->width += 2;
    }

    if (tileOnRight) {
        tilesOnEdges = true;
        grid->width += 2;
    }

    if (tilesOnEdges) {
        grid->tiles = (enum Color **) realloc(grid->tiles, grid->height * sizeof(enum Color *));

        for (int y = grid->height - 1; y >= 0; y--) {
            if ((tileOnTop && y == 0) || (adjustForOdd && y == 1) || (tileOnBottom && y == grid->height - 1)) {
                grid->tiles[y] = (enum Color *) calloc(grid->width, sizeof(enum Color));
            } else {
                if (tileOnTop) {
                    grid->tiles[y] = grid->tiles[y - (adjustForOdd ? 2 : 1)];
                }

                grid->tiles[y] = (enum Color *) realloc(grid->tiles[y], grid->width * sizeof(enum Color));

                for (int x = grid->width - 1; x >= 0; x--) {
                    if ((tileOnLeft && x < 2) || (tileOnRight && x > grid->width - 3)) {
                        grid->tiles[y][x] = WHITE;
                    } else {
                        grid->tiles[y][x] = grid->tiles[y][x - (tileOnLeft ? 2 : 0)];
                    }
                }
            }
        }
    }
}

void printGrid(struct Grid *grid) {
    for (int y = 0; y < grid->height; y++) {
        for (int x = 0; x < grid->width; x++) {
            if (grid->tiles[y][x] == BLACK) {
                printf("#");
            } else {
                printf(".");
            }
        }

        printf("\n");
    }
}

struct Grid *copyGrid(struct Grid *grid) {
    struct Grid *gridCopy = (struct Grid *) calloc(1, sizeof(struct Grid *));

    gridCopy->width = grid->width;
    gridCopy->height = grid->height;

    gridCopy->tiles = (enum Color **) calloc(gridCopy->height, sizeof(enum Color *));

    for (int y = 0; y < gridCopy->height; y++) {
        gridCopy->tiles[y] = (enum Color *) calloc(gridCopy->width, sizeof(enum Color));

        for (int x = 0; x < gridCopy->width; x++) {
            gridCopy->tiles[y][x] = grid->tiles[y][x];
        }
    }

    return gridCopy;
}

void updateGrid(struct Grid *grid, struct Grid *reference) {
    for (int y = 0; y < grid->height; y++) {
        for (int x = y % 2 == 0 ? 0 : 1; x < grid->width; x += 2) {
            int blackCount = 0;

            if (x < grid->width - 2 && reference->tiles[y][x + 2] == BLACK) {
                ++blackCount;
            }

            if (x < grid->width - 1 && y < grid->height - 1 && reference->tiles[y + 1][x + 1] == BLACK) {
                ++blackCount;
            }

            if (x > 0 && y < grid->height - 1 && reference->tiles[y + 1][x - 1] == BLACK) {
                ++blackCount;
            }

            if (x > 1 && reference->tiles[y][x - 2] == BLACK) {
                ++blackCount;
            }

            if (x < grid->width - 1 && y > 0 && reference->tiles[y - 1][x + 1] == BLACK) {
                ++blackCount;
            }

            if (x > 0 && y > 0 && reference->tiles[y - 1][x - 1] == BLACK) {
                ++blackCount;
            }

            switch (reference->tiles[y][x]) {
                case BLACK:
                    if (blackCount == 0 || blackCount > 2) {
                        grid->tiles[y][x] = WHITE;
                    }
                    break;
                case WHITE:
                    if (blackCount == 2) {
                        grid->tiles[y][x] = BLACK;
                    }
                    break;
            }
        }
    }
}

int countBlackGridTiles(struct Grid *grid) {
    int blackCount = 0;

    for (int y = 0; y < grid->height; y++) {
        for (int x = 0; x < grid->width; x++) {
            if (grid->tiles[y][x] == BLACK) {
                ++blackCount;
            }
        }
    }

    return blackCount;
}

void freeGrid(struct Grid *grid) {
    if (grid) {
        for (int y = 0; y < grid->height; y++) {
            free(grid->tiles[y]);
        }

        free(grid->tiles);
        free(grid);
    }
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct TileData *data = getTileData(aoc.input);
    int answer = 0;

    if (data) {
        int minX = 0;
        int maxX = 0;
        int minY = 0;
        int maxY = 0;

        getTileBounds(data->center, 0, 0, &minX, &maxX, &minY, &maxY);
        clearFlags(data->center);

        struct Grid *current = (struct Grid *) calloc(1, sizeof(struct Grid));
        current->width = maxX - minX + 1;
        current->height = maxY - minY + 1;

        current->tiles = (enum Color **) calloc(current->height, sizeof(enum Color *));

        for (int y = 0; y < current->height; y++) {
            current->tiles[y] = (enum Color *) calloc(current->width, sizeof(enum Color));
        }

        setBlackTiles(data->center, current->tiles, abs(minX), abs(minY));

        freeTileData(data);

        struct Grid *reference = NULL;

        for (int i = 0; i < 100; i++) {
            expandGrid(current);

            reference = copyGrid(current);

            updateGrid(current, reference);
            freeGrid(reference);
        }

        answer = countBlackGridTiles(current);

        freeGrid(current);
    }

    print(aoc, INT, &answer);

    return 0;
}
