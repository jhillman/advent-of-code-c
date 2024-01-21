/* Day 20, part 2 = 1909 */

#include "../../aoc.h"
#include <stdbool.h>
#include <math.h>
#include "tiles.h"

char tilePixel(struct TileData *data, struct ArrangedTile arrangedTile, int row, int column) {
    int tempColumn;

    for (int i = 0; i < arrangedTile.rotation; i++) {
        tempColumn = column;
        column = row;
        row = DATA_SIZE - tempColumn - 1;
    }

    if (arrangedTile.side == BACK) {
        column = DATA_SIZE - column - 1;
    }

    return data->tiles[arrangedTile.index].data[row][column];
}

char **rotate(char **image, int imageSize) {
    char **rotatedImage = (char **) calloc(imageSize, sizeof(char *));

    for (int i = 0; i < imageSize; i++) {
        rotatedImage[i] = (char *) calloc(imageSize + 1, sizeof(char));
    }

    for (int i = 0; i < imageSize; i++) {
        for (int j = 0; j < imageSize; j++) {
            rotatedImage[j][imageSize - i - 1] = image[i][j];
        }
    }

    for (int i = 0; i < imageSize; i++) {
        free(image[i]);
    }

    free(image);

    return rotatedImage;
}

void flip(char **image, int imageSize) {
    for (int i = 0; i < imageSize; i++) {
        int j = 0;
        int k = imageSize - 1;
        char ch;

        while (j < k) {
            ch = image[i][j];
            image[i][j] = image[i][k];
            image[i][k] = ch;
            j++;
            k--;
        }
    }
}

bool foundSeaMonster(char **image, int imageSize, int row, int column) {
    if (column > imageSize - 20) {
        return false;
    }

    int monsterRow = row;
    int monsterColumn = column;

    if (image[monsterRow][monsterColumn] != '#') {
        return false;
    }

    for (int i = 0; i < 3; i++) {
        monsterColumn += 5;

        if (image[monsterRow][monsterColumn++] != '#' || image[monsterRow][monsterColumn] != '#') {
            return false;
        }
    }

    if (image[monsterRow][monsterColumn + 1] == '#') {
        monsterRow = row - 1;
        monsterColumn = column + 18;
    } else {
        return false;
    }

    if (image[monsterRow][monsterColumn] == '#') {
        monsterRow = row + 1;
        monsterColumn = column + 1;
    } else {
        return false;
    }

    for (int i = 0; i < 6; i++) {
        if (image[monsterRow][monsterColumn] == '#') {
            monsterColumn += 3;
        } else {
            return false;
        }
    }

    return true;
}

void drawSeaMonster(char **image, int row, int column) {
    int monsterRow = row;
    int monsterColumn = column;

    image[monsterRow][monsterColumn] = 'O';

    for (int i = 0; i < 3; i++) {
        monsterColumn += 5;

        image[monsterRow][monsterColumn++] = 'O';
        image[monsterRow][monsterColumn] = 'O';
    }

    image[monsterRow][monsterColumn + 1] = 'O';

    monsterRow = row - 1;
    monsterColumn = column + 18;

    image[monsterRow][monsterColumn] = 'O';

    monsterRow = row + 1;
    monsterColumn = column + 1;

    for (int i = 0; i < 6; i++) {
        image[monsterRow][monsterColumn] = 'O';
        monsterColumn += 3; 
    }
}

bool imageHasSeaMonsters(char **image, int imageSize) {
    bool hasSeaMonsters = false;

    for (int row = 1; row < imageSize - 1; row++) {
        for (int column = 0; column < imageSize; column++) {
            if (foundSeaMonster(image, imageSize, row, column)) {
                drawSeaMonster(image, row, column);
                column += 20;

                hasSeaMonsters = true;
            }
        }
    }

    return hasSeaMonsters;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct TileData *data = getTileData(aoc.input);
    int answer = 0;

    if (data) {
        bool cornerFound = false;
        int cornerTileId;
        int cornerTileIndex;
        enum Side cornerTileSide;
        int cornerTileRotation;

        for (int i = 0; !cornerFound && i < data->tileCount; i++) {
            int verticalMatchCount = 0;
            int lastVerticalMatch = -1;

            for (int j = 0; j < data->verticalEdgeMatchCount; j++) {
                if (data->verticalEdgeMatches[j].tile1.id == data->tiles[i].id) {
                    if (data->verticalEdgeMatches[j].tile2.id != lastVerticalMatch) {
                        ++verticalMatchCount;

                        cornerTileId = data->tiles[i].id;
                        cornerTileIndex = data->tiles[i].index;
                        cornerTileSide = data->verticalEdgeMatches[j].tile1.side;
                        cornerTileRotation = data->verticalEdgeMatches[j].tile1.rotation;
                    }

                    lastVerticalMatch = data->verticalEdgeMatches[j].tile2.id;
                }
            }

            if (verticalMatchCount == 2) {
                cornerFound = true;
            }
        }

        struct ArrangedTile rowStartTile = { cornerTileId, cornerTileIndex, cornerTileSide, cornerTileRotation };
        struct ArrangedTile currentTile = rowStartTile;

        int gridSize = sqrt(data->tileCount);

        struct ArrangedTile arrangedTiles[gridSize][gridSize];
        **arrangedTiles = rowStartTile;

        int arrangedTileCount = 1;
        int tileRow = 0;
        int tileColumn = 1;

        data->tiles[currentTile.index].arranged = true;

         while (arrangedTileCount < data->tileCount) {
            bool nextTileFound = false;

            for (int i = 0; !nextTileFound && i < data->verticalEdgeMatchCount; i++) {
                struct EdgeMatch edgeMatch = data->verticalEdgeMatches[i];

                if (edgeMatch.tile1.id == currentTile.id && 
                    edgeMatch.tile1.side == currentTile.side &&
                    edgeMatch.tile1.rotation == currentTile.rotation &&
                    !data->tiles[edgeMatch.tile2.index].arranged) {
                    currentTile = arrangedTiles[tileRow][tileColumn++] = edgeMatch.tile2;

                    nextTileFound = true;
                    ++arrangedTileCount;
                } else if (edgeMatch.tile2.id == currentTile.id && 
                    edgeMatch.tile2.side == currentTile.side &&
                    edgeMatch.tile2.rotation == currentTile.rotation &&
                    !data->tiles[edgeMatch.tile1.index].arranged) {
                    currentTile = arrangedTiles[tileRow][tileColumn++] = edgeMatch.tile1;

                    nextTileFound = true;
                    ++arrangedTileCount;
                }
            }

            data->tiles[currentTile.index].arranged = true;

            if (tileColumn == gridSize) {
                ++tileRow;
                tileColumn = 0;
                bool nextTileFound = false;
    
                for (int i = 0; !nextTileFound && i < data->horizontalEdgeMatchCount; i++) {
                    struct EdgeMatch edgeMatch = data->horizontalEdgeMatches[i];

                    if (edgeMatch.tile1.id == rowStartTile.id && 
                        edgeMatch.tile1.side == rowStartTile.side &&
                        edgeMatch.tile1.rotation == rowStartTile.rotation &&
                        !data->tiles[edgeMatch.tile2.index].arranged) {
                        rowStartTile = currentTile = arrangedTiles[tileRow][tileColumn++] = edgeMatch.tile2;

                        nextTileFound = true;
                        ++arrangedTileCount;
                    } else if (edgeMatch.tile2.id == rowStartTile.id && 
                        edgeMatch.tile2.side == rowStartTile.side &&
                        edgeMatch.tile2.rotation == rowStartTile.rotation &&
                        !data->tiles[edgeMatch.tile1.index].arranged) {
                        currentTile = rowStartTile = arrangedTiles[tileRow][tileColumn++] = edgeMatch.tile1;

                        nextTileFound = true;
                        ++arrangedTileCount;
                    }
                }

                data->tiles[currentTile.index].arranged = true;
            }
        }

        int imageSize = gridSize * DATA_SIZE;

        char **image = (char **) calloc(imageSize, sizeof(char *));

        for (int row = 0; row < imageSize; row++) {
            image[row] = (char *) calloc(imageSize + 1, sizeof(char));
            
            for (int column = 0; column < imageSize; column++) {
                int arrangedTileRow = row / DATA_SIZE;
                int arrangedTileColumn = column / DATA_SIZE;
                struct ArrangedTile arrangedTile = arrangedTiles[arrangedTileRow][arrangedTileColumn];

                image[row][column] = tilePixel(data, arrangedTile, row % DATA_SIZE, column % DATA_SIZE);
            }
        }

        bool seaMonstersFound = false;

        while (!seaMonstersFound) {
            for (int i = 0; !seaMonstersFound && i < 3; i++) {
                seaMonstersFound = imageHasSeaMonsters(image, imageSize);

                if (!seaMonstersFound) {
                    image = rotate(image, imageSize);
                }
            }

            if (!seaMonstersFound) {
                image = rotate(image, imageSize);
                flip(image, imageSize);
            }
        }

        int waterRoughnessCount = 0;

        for (int row = 0; row < imageSize; row++) {
            for (int column = 0; column < imageSize; column++) {
                if (image[row][column] == '#') {
                    ++waterRoughnessCount;
                }
            }

            free(image[row]);
        }

        freeTileData(data);
        free(image);

        answer = waterRoughnessCount;
    }

    print(aoc, INT, &answer);

    return 0;
}
