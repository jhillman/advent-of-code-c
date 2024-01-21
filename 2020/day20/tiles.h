#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define DATA_SIZE 8
#define TILE_SIZE DATA_SIZE + 2

enum Side { FRONT, BACK };

struct Edges {
    int top;
    int bottom;
    int left;
    int right;
};

struct Tile {
    int id;
    int index;
    bool arranged;

    struct Edges edges[2][4];
    char data[DATA_SIZE][DATA_SIZE + 1];
};

struct ArrangedTile {
    int id;
    int index;
    enum Side side;
    int rotation;
};

struct EdgeMatch {
    struct ArrangedTile tile1;
    struct ArrangedTile tile2;
};

struct TileData {
    struct Tile *tiles;
    int tileCount;

    struct EdgeMatch *verticalEdgeMatches;
    int verticalEdgeMatchCount;
    
    struct EdgeMatch *horizontalEdgeMatches;
    int horizontalEdgeMatchCount;
};

int reverse(int value) {
    int reversedValue = 0;
    int bit = 1;
    int reversedBit = 1 << (TILE_SIZE - 1);

    for (int i = 0; i < TILE_SIZE; i++) {
        if ((value & bit) == bit) {
            reversedValue += reversedBit;
        }

        bit <<= 1;
        reversedBit >>= 1;
    }

    return reversedValue;    
}

struct TileData *getTileData(char *input) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        struct TileData *data = (struct TileData *) calloc(1, sizeof(struct TileData));

        data->tileCount = 0;
        char line[TILE_SIZE + 2];

        while (fgets(line, sizeof(line), inputFile)) {
            if (strstr(line, "Tile")) {
                ++data->tileCount;
            }
        }

        fseek(inputFile, 0, SEEK_SET);

        data->tiles = (struct Tile *) calloc(data->tileCount, sizeof(struct Tile));

        int tileIndex = 0;
        int position = 0;
        int bit;
        int top = 0;
        int bottom = 0;
        int left = 0;
        int right = 0;

        while (fgets(line, sizeof(line), inputFile)) {
            if (strstr(line, "Tile")) {
                sscanf(line, "Tile %d:", &data->tiles[tileIndex].id);
                data->tiles[tileIndex].index = tileIndex;
            } else if (strlen(line) <= 1) {
                position = 0;

                ++tileIndex;
            } else {
                if (position == 0) {
                    bit = 1;

                    for (int i = 0; i < TILE_SIZE; i++) {
                        if (line[TILE_SIZE - i - 1] == '#') {
                            top += bit;
                        }

                        bit <<= 1;
                    }

                    bit = 1 << (TILE_SIZE - 1);
                } else if (position == TILE_SIZE - 1) {
                    bit = 1;

                    for (int i = 0; i < TILE_SIZE; i++) {
                        if (line[TILE_SIZE - i - 1] == '#') {
                            bottom += bit;
                        }

                        bit <<= 1;
                    }

                    bit = 1;
                }

                if (position > 0 && position < TILE_SIZE - 1) {
                    strncpy(data->tiles[tileIndex].data[position - 1], line + 1, DATA_SIZE);
                }

                if (line[0] == '#') {
                    left += bit;
                }

                if (line[TILE_SIZE - 1] == '#') {
                    right += bit;
                }

                if (++position == TILE_SIZE) {
                    struct Edges edges = { 
                        top, 
                        bottom, 
                        left, 
                        right 
                    };
                    struct Edges backEdges = { 
                        reverse(top), 
                        reverse(bottom), 
                        right, 
                        left 
                    };

                    for (int i = 0; i < 4; i++) {
                        data->tiles[tileIndex].edges[FRONT][i] = edges;
                        data->tiles[tileIndex].edges[BACK][i] = backEdges;

                        edges = (struct Edges) {
                            reverse(edges.left),
                            reverse(edges.right),
                            edges.bottom,
                            edges.top
                        };

                        backEdges = (struct Edges) {
                            reverse(backEdges.left),
                            reverse(backEdges.right),
                            backEdges.bottom,
                            backEdges.top
                        };
                    }

                    top = 0;
                    bottom = 0;
                    left = 0;
                    right = 0;

                }

                bit >>= 1;
            }
        }

        fclose(inputFile);

        int verticalEdgeMatchSize = 100;
        data->verticalEdgeMatches = (struct EdgeMatch *) malloc(verticalEdgeMatchSize * sizeof(struct EdgeMatch));
        
        int horizontalEdgeMatchSize = 100;
        data->horizontalEdgeMatches = (struct EdgeMatch *) malloc(horizontalEdgeMatchSize * sizeof(struct EdgeMatch));

        data->verticalEdgeMatchCount = 0;
        data->horizontalEdgeMatchCount = 0;

        for (int i = 0; i < data->tileCount; i++) {
            struct Tile tile1 = data->tiles[i];

            for (int j = i + 1; j < data->tileCount; j++) {
                struct Tile tile2 = data->tiles[j];

                for (int k = 0; k < 2; k++) {
                    for (int l = 0; l < 4; l++) {
                        for (int m = 0; m < 2; m++) {
                            for (int n = 0; n < 4; n++) {
                                if (tile1.edges[k][l].right == tile2.edges[m][n].left) {
                                    data->verticalEdgeMatches[data->verticalEdgeMatchCount++] = (struct EdgeMatch) {
                                        { tile1.id, i, k, l },
                                        { tile2.id, j, m, n }
                                    };
                                }

                                if (tile1.edges[k][l].left == tile2.edges[m][n].right) {
                                    data->verticalEdgeMatches[data->verticalEdgeMatchCount++] = (struct EdgeMatch) {
                                        { tile2.id, j, m, n },
                                        { tile1.id, i, k, l }
                                    };
                                }

                                if (tile1.edges[k][l].bottom == tile2.edges[m][n].top) {
                                    data->horizontalEdgeMatches[data->horizontalEdgeMatchCount++] = (struct EdgeMatch) {
                                        { tile1.id, i, k, l },
                                        { tile2.id, j, m, n }
                                    };
                                }

                                if (tile1.edges[k][l].top == tile2.edges[m][n].bottom) {
                                    data->horizontalEdgeMatches[data->horizontalEdgeMatchCount++] = (struct EdgeMatch) {
                                        { tile2.id, j, m, n },
                                        { tile1.id, i, k, l }
                                    };
                                }

                                if (data->verticalEdgeMatchCount > verticalEdgeMatchSize - 10) {
                                    verticalEdgeMatchSize *= 1.5;
                                    data->verticalEdgeMatches = (struct EdgeMatch *) realloc(data->verticalEdgeMatches, verticalEdgeMatchSize * sizeof(struct EdgeMatch));
                                }

                                if (data->horizontalEdgeMatchCount > horizontalEdgeMatchSize - 10) {
                                    horizontalEdgeMatchSize *= 1.5;
                                    data->horizontalEdgeMatches = (struct EdgeMatch *) realloc(data->horizontalEdgeMatches, horizontalEdgeMatchSize * sizeof(struct EdgeMatch));
                                }
                            }
                        }
                    }
                }
            }
        }

        return data;
    }

    return NULL;
}

void freeTileData(struct TileData *data) {
    free(data->tiles);
    free(data->verticalEdgeMatches);
    free(data->horizontalEdgeMatches);
    free(data);
}
