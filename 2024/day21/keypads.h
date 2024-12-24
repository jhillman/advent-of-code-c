#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

#define min(a,b) ((a) < (b) ? (a) : (b))
#define MAX_LENGTH 100000000000L

struct XY {
    int x;
    int y;
};

struct KeyPads {
    int count;
    char ***data;
    long ***cache;
    struct XY *numberXYs;
    struct XY *directionXYs;
};

long sequenceLength(struct KeyPads *keyPads, int index, int count, ...) {
    long length = 0;
    char keys[8] = "";
    char **keyPad;
    char key;
    char next;
    struct XY *keyXYs;
    struct XY keyXY;
    struct XY nextXY;
    char xSequence[4];
    char ySequence[4];
    long xFirstLength;
    long yFirstLength;
    long movementLength;
    int keysLength;

    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; i++) {
        strcat(keys, va_arg(args, char *));
    }

    va_end(args);

    keysLength = strlen(keys);

    if (index == keyPads->count) {
        length = keysLength;
    } else {
        keyPad = keyPads->data[index];
        keyXYs = index == 0 ? keyPads->numberXYs : keyPads->directionXYs;
        key = 'A';

        for (int i = 0; i < keysLength; i++) {
            next = keys[i];

            if (keyPads->cache[key - '0'][next - '0'][index]) {
                movementLength = keyPads->cache[key - '0'][next - '0'][index];
            } else {
                keyXY = keyXYs[key - '0'];
                nextXY = keyXYs[next - '0'];

                memset(xSequence, 0, 4);
                memset(xSequence, nextXY.x < keyXY.x ? '<' : '>', abs(nextXY.x - keyXY.x));

                memset(ySequence, 0, 4);
                memset(ySequence, nextXY.y < keyXY.y ? '^' : 'v', abs(nextXY.y - keyXY.y));

                xFirstLength = keyPad[keyXY.y][nextXY.x] ? 
                    sequenceLength(keyPads, index + 1, 3, xSequence, ySequence, "A") : 
                    MAX_LENGTH;
                yFirstLength = keyPad[nextXY.y][keyXY.x] ? 
                    sequenceLength(keyPads, index + 1, 3, ySequence, xSequence, "A") : 
                    MAX_LENGTH;

                movementLength = min(xFirstLength, yFirstLength);

                keyPads->cache[key - '0'][next - '0'][index] = movementLength;
            }

            length += movementLength;

            key = next;
        }
    }

    return length;
}

struct KeyPads *getKeyPads(int robotCount) {
    struct KeyPads *keyPads = (struct KeyPads *)calloc(1, sizeof(struct KeyPads));

    char keyPad[4][3] = { 
        { '7', '8', '9' },
        { '4', '5', '6' },
        { '1', '2', '3' },
        {  0 , '0', 'A' }
    };

    char directionPad[2][3] = { 
        {  0 , '^', 'A' },
        { '<', 'v', '>' }
    };

    int keyRange = ('v' - '0') + 1;
    keyPads->count = robotCount + 1;

    keyPads->data = (char ***)malloc(keyPads->count * sizeof(char **));
    keyPads->numberXYs = (struct XY *)(calloc(keyRange, sizeof(struct XY)));
    keyPads->directionXYs = (struct XY *)(calloc(keyRange, sizeof(struct XY)));

    for (int i = 0; i < keyPads->count; i++) {
        if (i == 0) {
            keyPads->data[i] = (char **)malloc(4 * sizeof(char *));

            for (int y = 0; y < 4; y++) {
                keyPads->data[i][y] = (char *)malloc(3 * sizeof(char));

                for (int x = 0; x < 3; x++) {
                    keyPads->data[i][y][x] = keyPad[y][x];

                    if (keyPad[y][x]) {
                        keyPads->numberXYs[keyPad[y][x] - '0'] = (struct XY){ x, y };
                    }
                }
            }
        } else {
            keyPads->data[i] = (char **)malloc(2 * sizeof(char *));

            for (int y = 0; y < 2; y++) {
                keyPads->data[i][y] = (char *)malloc(3 * sizeof(char));

                for (int x = 0; x < 3; x++) {
                    keyPads->data[i][y][x] = directionPad[y][x];

                    if (directionPad[y][x]) {
                        keyPads->directionXYs[directionPad[y][x] - '0'] = (struct XY){ x, y };
                    }
                }
            }
        }
    }

    keyPads->cache = (long ***)malloc(keyRange * sizeof(long **));

    for (char k = '0'; k <= 'v'; k++) {
        keyPads->cache[k - '0'] = (long **)malloc(keyRange * sizeof(long *));

        for (char n = '0'; n <= 'v'; n++) {
            keyPads->cache[k - '0'][n - '0'] = (long *)malloc(keyPads->count * sizeof(long));
        }
    }

    return keyPads;
}

void freeKeyPads(struct KeyPads *keyPads) {
    for (int i = 0; i < keyPads->count; i++) {
        for (int y = 0; y < (i == 0 ? 4 : 2); y++) {
            free(keyPads->data[i][y]);
        }

        free(keyPads->data[i]);
    }

    for (char k = '0'; k <= 'v'; k++) {
        for (char n = '0'; n <= 'v'; n++) {
            free(keyPads->cache[k - '0'][n - '0']);
        }

        free(keyPads->cache[k - '0']);
    }

    free(keyPads->cache);
    free(keyPads->data);
    free(keyPads->numberXYs);
    free(keyPads->directionXYs);
    free(keyPads);
}

long getComplexitySum(char *input, int robotCount) {
    FILE *inputFile = fopen(input, "r");
    struct KeyPads *keyPads = getKeyPads(robotCount);
    long sum = 0;

    if (inputFile) {
        char keys[8];

        while (!feof(inputFile) && fgets(keys, sizeof(keys), inputFile)) {
            keys[strcspn(keys, "\n")] = 0;

            for (char k = '0'; k <= 'v'; k++) {
                for (char n = '0'; n <= 'v'; n++) {
                    memset(keyPads->cache[k - '0'][n - '0'], 0, keyPads->count * sizeof(long));
                }
            }

            sum += atol(keys) * sequenceLength(keyPads, 0, 1, keys);
        }

        fclose(inputFile);
    }

    freeKeyPads(keyPads);

    return sum;    
}
