#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

struct Astroid {
    int x;
    int y;
};

struct Delta {
    int dx;
    int dy;
};

struct AstroidData {
    struct Astroid astroid;
    struct Delta delta;
    float angle;
    float distance;
    bool vaporized;
};

int delta(int v1, int v2) {
    return v1 > v2 ? 1 : (v2 > v1 ? -1 : 0);
}

struct Astroid *getAstroids(char *input, int *count) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        char ch;
        int width = 0;
        int height = 1;
        int astroidCapacity = 10;
        int astroidCount = 0;
        struct Astroid *astroids = (struct Astroid *)malloc(astroidCapacity * sizeof(struct Astroid));

        while (fscanf(inputFile, "%c", &ch) >= 1) {
            if (ch == '\n') {
                ++height;
            } else if (height == 1) {
                ++width;
            }
        }

        fseek(inputFile, 0, SEEK_SET);

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                fscanf(inputFile, "%c", &ch);

                if (ch == '#') {
                    astroids[astroidCount++] = (struct Astroid){ x, y };

                    if (astroidCount == astroidCapacity) {
                        astroidCapacity += 10;

                        astroids = (struct Astroid *)realloc(astroids, astroidCapacity * sizeof(struct Astroid));
                    }
                }
            }

            fscanf(inputFile, "%c", &ch); // \n
        }

        fclose(inputFile);

        *count = astroidCount;

        return astroids;
    }

    return NULL;
}

struct AstroidData *astroidData(struct Astroid astroid, struct Astroid *astroids, int astroidCount, bool visibleOnly, int *count) {
    int astroidDataCapacity = 10;
    int astroidDataCount = 0;
    struct AstroidData *astroidData = (struct AstroidData *)malloc(astroidDataCapacity * sizeof(struct AstroidData));

    for (int i = 0; i < astroidCount; i++) {
        if (!(astroid.x == astroids[i].x && astroid.y == astroids[i].y)) {
            int dx = delta(astroids[i].x, astroid.x);
            int dy = delta(astroids[i].y, astroid.y);
            float angle = ((float)astroids[i].y - (float)astroid.y) / ((float)astroids[i].x - (float)astroid.x);
            float distance = fabs(sqrt(pow((float)astroids[i].x - (float)(astroid.x), 2) + pow((float)astroids[i].y - (float)astroid.y, 2)));
            bool astroidDataFound = false;

            if (visibleOnly) {
                for (int j = 0; !astroidDataFound && j < astroidDataCount; j++) {
                    if (astroidData[j].delta.dx == dx && astroidData[j].delta.dy == dy && astroidData[j].angle == angle) {
                        astroidDataFound = true;
                    }
                }
            }

            if (!astroidDataFound || !visibleOnly) {
                astroidData[astroidDataCount++] = (struct AstroidData){ astroids[i], (struct Delta){ dx, dy }, angle, distance, false };

                if (astroidDataCount == astroidDataCapacity) {
                    astroidDataCapacity += 10;

                    astroidData = (struct AstroidData *)realloc(astroidData, astroidDataCapacity * sizeof(struct AstroidData));
                }
            }
        }
    }

    *count = astroidDataCount;

    return astroidData;
}