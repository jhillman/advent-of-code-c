#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

#define CAVERN_WIDTH 7

enum RockType {
    MINUS,
    PLUS,
    SHELF,
    LINE,
    BLOCK
};

struct Rock {
    int *state;
    int rowCount;
    long top;
    long left;
    long right;
    long bottom;
};

struct Cavern {
    FILE *inputFile;
    int *cavern;
    long cavernHeight;
    long top;
    enum RockType rockType;
    struct Rock rock;
    int gustIndex;
    long rockCount;
};

int bitCount(int number) {
    int count = 0;
    
    for (int i = 0; i < CAVERN_WIDTH; i++) {
        if (number & (1 << i)) {
            ++count;
        }
    }

    return count;
}

struct Rock addRock(struct Cavern *cavern) {
    int *state;

    switch (cavern->rockType) {
    case MINUS:
        state = (int *)malloc(1 * sizeof(int));
        state[0] = cavern->cavern[cavern->top + 3] = 0b0011110;

        return (struct Rock){
            state,
            1,
            cavern->top + 3,
            2, 5,
            cavern->top + 3
        };
    case PLUS:
        state = (int *)malloc(3 * sizeof(int));
        state[2] = cavern->cavern[cavern->top + 5] = 0b0001000;
        state[1] = cavern->cavern[cavern->top + 4] = 0b0011100;
        state[0] = cavern->cavern[cavern->top + 3] = 0b0001000;

        return (struct Rock){
            state,
            3,
            cavern->top + 5,
            2, 4,
            cavern->top + 3
        };
    case SHELF:
        state = (int *)malloc(3 * sizeof(int));
        state[2] = cavern->cavern[cavern->top + 5] = 0b0000100;
        state[1] = cavern->cavern[cavern->top + 4] = 0b0000100;
        state[0] = cavern->cavern[cavern->top + 3] = 0b0011100;

        return (struct Rock){
            state,
            3,
            cavern->top + 5,
            2, 4,
            cavern->top + 3
        };
    case LINE:
        state = (int *)malloc(4 * sizeof(int));
        state[3] = cavern->cavern[cavern->top + 6] = 0b0010000;
        state[2] = cavern->cavern[cavern->top + 5] = 0b0010000;
        state[1] = cavern->cavern[cavern->top + 4] = 0b0010000;
        state[0] = cavern->cavern[cavern->top + 3] = 0b0010000;

        return (struct Rock){
            state,
            4,
            cavern->top + 6,
            2, 2,
            cavern->top + 3
        };
    case BLOCK:
        state = (int *)malloc(2 * sizeof(int));
        state[1] = cavern->cavern[cavern->top + 4] = 0b0011000;
        state[0] = cavern->cavern[cavern->top + 3] = 0b0011000;

        return (struct Rock){
            state,
            2,
            cavern->top + 4,
            2, 3,
            cavern->top + 3
        };
        break;
    }
}

void printCavern(struct Cavern *cavern) {
    for (int i = cavern->top; i >= 0; i--) {
        printf("|");

        for (int j = CAVERN_WIDTH - 1; j >= 0; j--) {
            if ((1 << j) & cavern->cavern[i]) {
                printf("#");
            } else {
                printf(".");
            }
        }

        printf("|\n");
    }

    printf("+");

    for (int i = 0; i < CAVERN_WIDTH; i++) {
        printf("-");
    }

    printf("+\n");
}

void tryMoveLeft(struct Rock *rock, struct Cavern *cavern) {
    bool canMove = rock->left > 0;

    if (canMove && rock->bottom <= cavern->top) {
        for (int i = 0; canMove && i < rock->rowCount; i++) {
            if (rock->bottom + i - 1 <= cavern->top) {
                canMove = bitCount(cavern->cavern[rock->bottom + i]) == 
                    bitCount((cavern->cavern[rock->bottom + i] ^ rock->state[i]) | rock->state[i] << 1);
            }
        }        
    }

    if (canMove) {
        --rock->left;
        --rock->right;

        for (int i = 0; i < rock->rowCount; i++) {
            cavern->cavern[rock->bottom + i] ^= rock->state[i];
            cavern->cavern[rock->bottom + i] |= (rock->state[i] <<= 1);
        }
    }
}

void tryMoveRight(struct Rock *rock, struct Cavern *cavern) {
    bool canMove = rock->right < CAVERN_WIDTH - 1;

    if (canMove && rock->bottom <= cavern->top) {
        for (int i = 0; canMove && i < rock->rowCount; i++) {
            if (rock->bottom + i - 1 <= cavern->top) {
                canMove = bitCount(cavern->cavern[rock->bottom + i]) == 
                    bitCount((cavern->cavern[rock->bottom + i] ^ rock->state[i]) | rock->state[i] >> 1);
            }
        }        
    }

    if (canMove) {
        ++rock->left;
        ++rock->right;

        for (int i = 0; i < rock->rowCount; i++) {
            cavern->cavern[rock->bottom + i] ^= rock->state[i];
            cavern->cavern[rock->bottom + i] |= (rock->state[i] >>= 1);
        }
    }
}

bool tryMoveDown(struct Rock *rock, struct Cavern *cavern) {
    bool canMove = rock->bottom > 0;

    if (canMove && rock->bottom <= cavern->top + 1) {
        for (int i = 0; canMove && i < rock->rowCount; i++) {
            if (rock->bottom + i - 1 <= cavern->top) {
                canMove = bitCount(cavern->cavern[rock->bottom + i - 1] ^ (i > 0 ? rock->state[i - 1] : 0)) + bitCount(rock->state[i]) == 
                    bitCount((cavern->cavern[rock->bottom + i - 1] ^ (i > 0 ? rock->state[i - 1] : 0)) | rock->state[i]);
            }
        }        
    }

    if (canMove) {
        for (int i = 0; i < rock->rowCount; i++) {
            cavern->cavern[rock->bottom + i] ^= rock->state[i];
            cavern->cavern[rock->bottom + i - 1] |= rock->state[i];
        }        

        --rock->top;
        --rock->bottom;
    }

    return canMove;
}

struct CavernState {
    long cavernTop;
    long rockCount;
    enum RockType rockType;
    int gustIndex;
    int row;
};

int compareStates(struct CavernState stateOne, struct CavernState stateTwo) {
    int result = stateOne.rockType - stateTwo.rockType;

    if (result == 0) {
        result = stateOne.gustIndex - stateTwo.gustIndex;
    }

    if (result == 0) {
        result = stateOne.row - stateTwo.row;
    }

    return result;
}

int binarySearchStates(struct CavernState cavernState, struct CavernState *cavernStates, int low, int high) {
    if (low > high) {
        return -1;
    }

    int middle = low + ((high - low) / 2);

    int result = compareStates(cavernStates[middle], cavernState);

    if (result == 0) {
        return middle;
    } else if (result < 0) {
        return binarySearchStates(cavernState, cavernStates, low, middle - 1);
    } else {
        return binarySearchStates(cavernState, cavernStates, middle + 1, high);
    }
}

int compare(const void *a, const void *b) {
    return compareStates(*(struct CavernState *)a, *(struct CavernState *)b);
}

struct Cavern *getCavern(char *input) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        struct Cavern *cavern = (struct Cavern *)calloc(1, sizeof(struct Cavern));
        cavern->inputFile = inputFile;
        cavern->cavernHeight = 1000;
        cavern->cavern = (int *)malloc(cavern->cavernHeight * sizeof(int));
        cavern->rockType = MINUS;

        return cavern;
    }

    return NULL;    
}

void freeCavern(struct Cavern *cavern) {
    free(cavern->cavern);
    free(cavern);
}

void dropRocks(struct Cavern *cavern, int targetRockCount) {
    int rockCount = 0;

    cavern->rock = addRock(cavern);

    while (rockCount < targetRockCount) {
        char gust = fgetc(cavern->inputFile);

        if (feof(cavern->inputFile)) {
            fseek(cavern->inputFile, 0, SEEK_SET);

            gust = fgetc(cavern->inputFile);

            cavern->gustIndex = 0;
        }

        if (gust == '<') {
            tryMoveLeft(&cavern->rock, cavern);
        } else {
            tryMoveRight(&cavern->rock, cavern);
        }

        if (!tryMoveDown(&cavern->rock, cavern)) {
            cavern->top = max(cavern->top, cavern->rock.bottom + cavern->rock.rowCount);

            if (cavern->rockType++ == BLOCK) {
                cavern->rockType = MINUS;
            }

            if (cavern->cavernHeight - cavern->top < 10) {
                cavern->cavernHeight += 1000;

                cavern->cavern = (int *)realloc(cavern->cavern, cavern->cavernHeight * sizeof(int));
                memset(cavern->cavern + cavern->cavernHeight - 1000, 0, 1000 * sizeof(int));
            }

            free(cavern->rock.state);

            ++cavern->rockCount;

            if (++rockCount < targetRockCount) {
                cavern->rock = addRock(cavern);
            }
        }

        ++cavern->gustIndex;
    }
}
