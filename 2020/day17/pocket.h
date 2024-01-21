#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

struct PocketDimension {
    int width;
    int height;
    int depth;
    int trength;
    bool fourDimensions;
    char ****cubes;
};

struct PocketDimension *initializePocketDimension(int width, int height, int depth, int trength) {
    struct PocketDimension *pocketDimension = (struct PocketDimension *) calloc(1, sizeof(struct PocketDimension));
    pocketDimension->width = width;
    pocketDimension->height = height;
    pocketDimension->depth = depth;
    pocketDimension->trength = max(trength, 1);
    pocketDimension->fourDimensions = trength > 0;

    pocketDimension->cubes = (char ****) calloc(pocketDimension->trength, sizeof(char ****));

    for (int w = 0; w < pocketDimension->trength; w++) {
        pocketDimension->cubes[w] = (char ***) calloc(pocketDimension->depth, sizeof(char ***));

        for (int z = 0; z < pocketDimension->depth; z++) {
            pocketDimension->cubes[w][z] = (char **) calloc(pocketDimension->height, sizeof(char **));
    
            for (int y = 0; y < pocketDimension->height; y++) {
                pocketDimension->cubes[w][z][y] = (char *) calloc(pocketDimension->width + 1, sizeof(char));
                memset(pocketDimension->cubes[w][z][y], '.', pocketDimension->width);
             }
        }
    }

    return pocketDimension;
}

struct PocketDimension *readPocketDimension(char *input, bool fourDimensions) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        char ch = fgetc(inputFile);
        int width = 0;
        int height = 0;

        while (ch != '\n') {
            ++width;
            ch = fgetc(inputFile);
        }

        fseek(inputFile, 0, SEEK_SET);

        while (!feof(inputFile)) {
            ch = fgetc(inputFile);

            if (ch == '\n') {
                ++height;
            }
        }

        ++height;

        struct PocketDimension *pocketDimension = initializePocketDimension(width, height, 1, fourDimensions ? 1 : 0);

        fseek(inputFile, 0, SEEK_SET);

        int y = 0;

        while (y < pocketDimension->height) {
            fscanf(inputFile, "%s", pocketDimension->cubes[0][0][y++]);
        }

        fclose(inputFile);

        return pocketDimension;
    }

    return NULL;
}

void printPocketDimension(struct PocketDimension *pocketDimension) {
    for (int w = 0; w < pocketDimension->trength; w++) {
        for (int z = 0; z < pocketDimension->depth; z++) {
            printf("z: %d, w: %d\n", z, w);

            for (int y = 0; y < pocketDimension->height; y++) {
                printf("%s\n", pocketDimension->cubes[w][z][y]);
            }    

            printf("\n");
        }
    }

    printf("\n");
}

void freePocketDimension(struct PocketDimension *pocketDimension) {
    for (int w = 0; w < pocketDimension->trength; w++) {
        for (int z = 0; z < pocketDimension->depth; z++) {
            for (int y = 0; y < pocketDimension->height; y++) {
                free(pocketDimension->cubes[w][z][y]);
            }

            free(pocketDimension->cubes[w][z]);
        }

        free(pocketDimension->cubes[w]);
    }

    free(pocketDimension->cubes);
    free(pocketDimension);
}

char cubeState(struct PocketDimension *pocketDimension, int x, int y, int z, int w) {
    int activeNeighborCount = 0;
    char state = '.';

    for (int nW = max(0, w - 1); nW <= min(pocketDimension->trength - 1, w + 1); nW++) {
        for (int nZ = max(0, z - 1); nZ <= min(pocketDimension->depth - 1, z + 1); nZ++) {
            for (int nY = max(0, y - 1); nY <= min(pocketDimension->height - 1, y + 1); nY++) {
                for (int nX = max(0, x - 1); nX <= min(pocketDimension->width - 1, x + 1); nX++) {
                    if (!(nW == w && nZ == z && nY == y && nX == x)) {
                        if (pocketDimension->cubes[nW][nZ][nY][nX] == '#') {
                            ++activeNeighborCount;
                        }
                    }
                }    
            }
        }
    }    

    switch (pocketDimension->cubes[w][z][y][x]) {
        case '#':
            if (activeNeighborCount == 2 || activeNeighborCount == 3) {
                state = '#';
            }
            break;
        case '.':
            if (activeNeighborCount == 3) {
                state = '#';
            }
            break;
    }

    return state;
}

struct PocketDimension *cyclePocketDimension(struct PocketDimension *pocketDimension) {
    struct PocketDimension *expandedPocketDimension = initializePocketDimension(
        pocketDimension->width + 2, 
        pocketDimension->height + 2, 
        pocketDimension->depth + 2,
        pocketDimension->fourDimensions ? pocketDimension->trength + 2 : 0);

    int wOffset = pocketDimension->fourDimensions ? 1 : 0;

    for (int w = 0; w < pocketDimension->trength; w++) {
        for (int z = 0; z < pocketDimension->depth; z++) {
            for (int y = 0; y < pocketDimension->height; y++) {
                for (int x = 0; x < pocketDimension->width; x++) {
                    expandedPocketDimension->cubes[w + wOffset][z + 1][y + 1][x + 1] = pocketDimension->cubes[w][z][y][x];
                }    
            }
        }    
    }

    struct PocketDimension *cycledPocketDimension = initializePocketDimension(
        pocketDimension->width + 2, 
        pocketDimension->height + 2, 
        pocketDimension->depth + 2,
        pocketDimension->fourDimensions ? pocketDimension->trength + 2 : 0);

    for (int w = 0; w < cycledPocketDimension->trength; w++) {
        for (int z = 0; z < cycledPocketDimension->depth; z++) {
            for (int y = 0; y < cycledPocketDimension->height; y++) {
                for (int x = 0; x < cycledPocketDimension->width; x++) {
                    cycledPocketDimension->cubes[w][z][y][x] = cubeState(expandedPocketDimension, x, y, z, w);
                }    
            }    
        }
    }

    freePocketDimension(expandedPocketDimension);
    freePocketDimension(pocketDimension);

    return cycledPocketDimension;
}

int activeCubeCount(struct PocketDimension *pocketDimension) {
    int activeCount = 0;

    for (int w = 0; w < pocketDimension->trength; w++) {
        for (int z = 0; z < pocketDimension->depth; z++) {
            for (int y = 0; y < pocketDimension->height; y++) {
                for (int x = 0; x < pocketDimension->width; x++) {
                    if (pocketDimension->cubes[w][z][y][x] == '#') {
                        ++activeCount;
                    }
                }    
            }    
        }
    }

    return activeCount;
}
