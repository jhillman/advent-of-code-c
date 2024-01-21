#include "../intcode.h"

#define STOP_GAME -1000L
#define GRID_WIDTH 50
#define GRID_HEIGHT 25

enum TileId {
    EMPTY,
    WALL,
    BLOCK,
    PADDLE,
    BALL
};

int **initializeTiles() {
    int **tiles = (int **)calloc(GRID_HEIGHT, sizeof(int *));

    for (int y = 0; y < GRID_HEIGHT; y++) {
        tiles[y] = (int *)calloc(GRID_WIDTH, sizeof(int));            
    }

    return tiles;
}

void freeTiles(int **tiles) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        free(tiles[y]);
    }

    free(tiles);
}
