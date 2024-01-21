#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

struct Check {
    int x;
    int y;
};

struct Move {
    struct Check checks[3];
    int dx;
    int dy;
};

struct MoveProposal {
    int fromX;
    int fromY;
    int toX;
    int toY;
};

struct GroveData {
    int width;
    int height;
    char **grove;
    int minElfX;
    int maxElfX;
    int minElfY;
    int maxElfY;
    struct Move moves[4];
    int moveStartIndex;
    struct MoveProposal *proposals;
};

int compare(const void *a, const void *b) {
    struct MoveProposal *moveA = (struct MoveProposal *)a; 
    struct MoveProposal *moveB = (struct MoveProposal *)b;

    int result = moveA->toX - moveB->toX;

    if (result == 0) {
        result = moveA->toY - moveB->toY;
    }

    return result;
}

struct GroveData *getGroveData(char *input) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        struct GroveData *data = (struct GroveData *)calloc(1, sizeof(struct GroveData));
        int elfCount = 0;

        char c = fgetc(inputFile);

        while (!feof(inputFile)) {
            if (c == '\n') {
                ++data->height;
            }

            if (!data->height) {
                ++data->width;
            }

            c = fgetc(inputFile);
        }

        ++data->height;

        fseek(inputFile, 0, SEEK_SET);

        data->grove = (char **)calloc(data->height, sizeof(char *));
        data->minElfX = data->width;
        data->maxElfX = 0;
        data->minElfY = data->height;
        data->maxElfY = 0;

        for (int y = 0; y < data->height; y++) {
            data->grove[y] = (char *)calloc(data->width, sizeof(char));

            for (int x = 0; x < data->width; x++) {
                data->grove[y][x] = fgetc(inputFile);

                if (data->grove[y][x] == '#') {
                    ++elfCount;

                    data->minElfX = min(x, data->minElfX);
                    data->maxElfX = max(x, data->maxElfX);
                    data->minElfY = min(y, data->minElfY);
                    data->maxElfY = max(y, data->maxElfY);
                }
            }

            if (y < data->height) {
                fgetc(inputFile); // \n
            }
        }

        fclose(inputFile);
        
        data->moves[0] = (struct Move){ -1, -1 , 0, -1, 1, -1, 0, -1 };
        data->moves[1] = (struct Move){ -1, 1, 0, 1, 1, 1, 0, 1 };
        data->moves[2] = (struct Move){ -1, -1, -1, 0, -1, 1, -1, 0 };
        data->moves[3] = (struct Move){ 1, -1, 1, 0, 1, 1, 1, 0 };

        data->proposals = (struct MoveProposal *)malloc(elfCount * sizeof(struct MoveProposal));

        return data;
    }    

    return NULL;
}

void freeGroveData(struct GroveData *data) {
    for (int y = 0; y < data->height; y++) {
        free(data->grove[y]);
    }

    free(data->grove);
    free(data->proposals);
    free(data);
}

int moveElves(struct GroveData *data) {
    int elvesMoved = 0;

    bool growNorth = data->minElfY == 0;
    bool growSouth = data->maxElfY == data->height - 1;
    bool growWest = data->minElfX == 0;
    bool growEast = data->maxElfX == data->width - 1;

    if (growNorth || growSouth) {
        data->height += ((growNorth ? 1 : 0) + (growSouth ? 1 : 0));

        data->grove = (char **)realloc(data->grove, data->height * sizeof(char *));

        if (growNorth) {
            memcpy(data->grove + 1, data->grove, (data->height - 1) * sizeof(char *));
            *data->grove = (char *)malloc(data->width * sizeof(char));

            memset(*data->grove, '.', data->width);
        }

        if (growSouth) {
            data->grove[data->height - 1] = (char *)malloc(data->width * sizeof(char));

            memset(data->grove[data->height - 1], '.', data->width);
        }
    }

    if (growWest || growEast) {
        data->width += ((growWest ? 1 : 0) + (growEast ? 1 : 0));

        for (int y = 0; y < data->height; y++) {
            data->grove[y] = (char *)realloc(data->grove[y], data->width * sizeof(char));

            if (growWest) {
                memcpy(data->grove[y] + 1, data->grove[y], (data->width - 1) * sizeof(char));
                *data->grove[y] = '.';
            }

            if (growEast) {
                data->grove[y][data->width - 1] = '.';
            }
        }
    }

    int proposalCount = 0;

    for (int y = 0; y < data->height; y++) {
        for (int x = 0; x < data->width; x++) {
            if (data->grove[y][x] == '#') { 
                bool elvesAdjacent = false;

                for (int i = 0; !elvesAdjacent && i < 4; i++) {
                    struct Move move = data->moves[i];

                    for (int j = 0; !elvesAdjacent && j < 3; j++) {
                        elvesAdjacent = data->grove[y + move.checks[j].y][x + move.checks[j].x] == '#';
                    }
                }

                if (elvesAdjacent) {
                    bool moveProposed = false;
                    int moveIndex = data->moveStartIndex;

                    for (int i = 0; !moveProposed && i < 4; i++) {
                        struct Move move = data->moves[moveIndex];
                        bool elfFound = false;

                        for (int j = 0; !elfFound && j < 3; j++) {
                            elfFound = data->grove[y + move.checks[j].y][x + move.checks[j].x] == '#';
                        }

                        if (!elfFound) {
                            data->proposals[proposalCount++] = (struct MoveProposal){ x, y, x + move.dx, y + move.dy };
                            moveProposed = true;
                        }

                        if (++moveIndex == 4) {
                            moveIndex = 0;
                        }
                    }
                }
            }
        }
    }

    qsort(data->proposals, proposalCount, sizeof(struct MoveProposal), compare);

    for (int i = 0; i < proposalCount; i++) {
        int proposalIndex = i;

        while (i < proposalCount - 1 && data->proposals[i].toX == data->proposals[i + 1].toX && data->proposals[i].toY == data->proposals[i + 1].toY) {
            ++i;
        }

        if (proposalIndex == i) {
            data->grove[data->proposals[i].fromY][data->proposals[i].fromX] = '.';
            data->grove[data->proposals[i].toY][data->proposals[i].toX] = '#';

            ++elvesMoved;
        }
    }

    data->minElfX = data->width;
    data->maxElfX = 0;
    data->minElfY = data->height;
    data->maxElfY = 0;

    for (int y = 0; y < data->height; y++) {
        for (int x = 0; x < data->width; x++) {
            if (data->grove[y][x] == '#') { 
                data->minElfX = min(x, data->minElfX);
                data->maxElfX = max(x, data->maxElfX);
                data->minElfY = min(y, data->minElfY);
                data->maxElfY = max(y, data->maxElfY);
            }
        }
    }

    if (++data->moveStartIndex == 4) {
        data->moveStartIndex = 0;
    }

    return elvesMoved;
}
