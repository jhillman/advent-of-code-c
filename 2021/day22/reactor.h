#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

struct Cuboid {
    long xMin;
    long xMax;
    long yMin;
    long yMax;
    long zMin;
    long zMax;
    bool on;
    bool sliced;
};

struct CuboidList {
    struct Cuboid *list;
    int size;
};

long cuboidVolume(struct Cuboid cuboid) {
    return ((cuboid.xMax + 1) - cuboid.xMin) * ((cuboid.yMax + 1) - cuboid.yMin) * ((cuboid.zMax + 1) - cuboid.zMin);
}

void addCuboid(struct CuboidList *list, struct Cuboid cuboid) {
    if (++list->size == 1) {
        list->list = (struct Cuboid *)malloc(list->size * sizeof(struct Cuboid));
    } else {
        list->list = (struct Cuboid *)realloc(list->list, list->size * sizeof(struct Cuboid));
    }

    list->list[list->size - 1] = cuboid;
}

bool cuboidsIntersect(struct Cuboid first, struct Cuboid second) {
    return first.xMin <= second.xMax && first.xMax >= second.xMin &&
           first.yMin <= second.yMax && first.yMax >= second.yMin &&
           first.zMin <= second.zMax && first.zMax >= second.zMin;
}

void sliceCuboid(struct Cuboid cuboid, struct Cuboid intersection, struct CuboidList *onCuboids) {
    if (cuboid.xMin < intersection.xMin) { // left
        addCuboid(onCuboids, (struct Cuboid) {
            cuboid.xMin,
            intersection.xMin - 1,
            cuboid.yMin,
            cuboid.yMax,
            cuboid.zMin,
            cuboid.zMax,
            true,
            false
        });

        cuboid.xMin = intersection.xMin;
    }

    if (cuboid.xMax > intersection.xMax) { // right
        addCuboid(onCuboids, (struct Cuboid) {
            intersection.xMax + 1,
            cuboid.xMax,
            cuboid.yMin,
            cuboid.yMax,
            cuboid.zMin,
            cuboid.zMax,
            true,
            false
        });

        cuboid.xMax = intersection.xMax;
    }

    if (cuboid.yMin < intersection.yMin) { // top
        addCuboid(onCuboids, (struct Cuboid) {
            cuboid.xMin,
            cuboid.xMax,
            cuboid.yMin,
            intersection.yMin - 1,
            cuboid.zMin,
            cuboid.zMax,
            true,
            false
        });

        cuboid.yMin = intersection.yMin;
    }

    if (cuboid.yMax > intersection.yMax) { // bottom
        addCuboid(onCuboids, (struct Cuboid) {
            cuboid.xMin,
            cuboid.xMax,
            intersection.yMax + 1,
            cuboid.yMax,
            cuboid.zMin,
            cuboid.zMax,
            true,
            false
        });

        cuboid.yMax = intersection.yMax;
    }

    if (cuboid.zMin < intersection.zMin) { // back
        addCuboid(onCuboids, (struct Cuboid) {
            cuboid.xMin,
            cuboid.xMax,
            cuboid.yMin,
            cuboid.yMax,
            cuboid.zMin,
            intersection.zMin - 1,
            true,
            false
        });

        cuboid.zMin = intersection.zMin;
    }

    if (cuboid.zMax > intersection.zMax) { // front
        addCuboid(onCuboids, (struct Cuboid) {
            cuboid.xMin,
            cuboid.xMax,
            cuboid.yMin,
            cuboid.yMax,
            intersection.zMax + 1,
            cuboid.zMax,
            true,
            false
        });

        cuboid.zMax = intersection.zMax;
    }
}

long rebootReactor(char *input, bool initializeOnly) {
    FILE *inputFile = fopen(input, "r");
    long answer = 0;

    if (inputFile) {
        char state[4];
        struct Cuboid cuboid;
        struct CuboidList *cuboids = (struct CuboidList *)calloc(1, sizeof(struct CuboidList));
        struct CuboidList *onCuboids = (struct CuboidList *)calloc(1, sizeof(struct CuboidList));

        while (fscanf(inputFile, "%s x=%ld..%ld,y=%ld..%ld,z=%ld..%ld", 
                      state, &cuboid.xMin, &cuboid.xMax, &cuboid.yMin, &cuboid.yMax, &cuboid.zMin, &cuboid.zMax) >= 0) {
            cuboid.xMin = min(cuboid.xMin, cuboid.xMax);
            cuboid.xMax = max(cuboid.xMin, cuboid.xMax);
            cuboid.yMin = min(cuboid.yMin, cuboid.yMax);
            cuboid.yMax = max(cuboid.yMin, cuboid.yMax);
            cuboid.zMin = min(cuboid.zMin, cuboid.zMax);
            cuboid.zMax = max(cuboid.zMin, cuboid.zMax);
            cuboid.on = strcmp(state, "on") == 0;
            cuboid.sliced = false;

            if (!initializeOnly || labs(cuboid.xMin) <= 50) {
                addCuboid(cuboids, cuboid);
            }
        }

        fclose(inputFile);

        for (int i = 0; i < cuboids->size; i++) {
            for (int j = 0; j < onCuboids->size; j++) {
                if (!onCuboids->list[j].sliced && cuboidsIntersect(onCuboids->list[j], cuboids->list[i])) {
                    onCuboids->list[j].sliced = true;
                    sliceCuboid(onCuboids->list[j], cuboids->list[i], onCuboids);
                }
            }

            if (cuboids->list[i].on) {
                addCuboid(onCuboids, cuboids->list[i]);
            }
        }

        for (int i = 0; i < onCuboids->size; i++) {
            if (!onCuboids->list[i].sliced) {
                answer += cuboidVolume(onCuboids->list[i]);
            }
        }

        free(cuboids->list);
        free(cuboids);

        free(onCuboids->list);
        free(onCuboids);
    }

    return answer;
}
