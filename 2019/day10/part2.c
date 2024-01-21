/* Day 10, part 2 = 1205 */

#include "../../aoc.h"
#include "astroids.h"

int compare(const void *a, const void *b) {
    float angleA = (*(struct AstroidData **)a)->angle;
    float angleB = (*(struct AstroidData **)b)->angle;

    int result = (angleA > angleB) - (angleA < angleB);

    if (result == 0) {
        float distanceA = (*(struct AstroidData **)a)->distance;
        float distanceB = (*(struct AstroidData **)b)->distance;

        result = (distanceA > distanceB) - (distanceA < distanceB);
    }

    return result;
}

struct AstroidData **rotationAstroidData(struct Delta rotation, struct AstroidData *data, int dataCount, int *rotationDataCount) {
    int astroidDataCapacity = 10;
    int astroidDataCount = 0;
    struct AstroidData **astroidData = (struct AstroidData **)malloc(astroidDataCapacity * sizeof(struct AstroidData *));


    for (int i = 0; i < dataCount; i++) {
        if (!data[i].vaporized && data[i].delta.dx == rotation.dx && data[i].delta.dy == rotation.dy) {
            astroidData[astroidDataCount++] = &data[i];

            if (astroidDataCount == astroidDataCapacity) {
                astroidDataCapacity += 10;

                astroidData = (struct AstroidData **)realloc(astroidData, astroidDataCapacity * sizeof(struct AstroidData *));
            }
        }
    }

    qsort(astroidData, astroidDataCount, sizeof(struct AstroidData *), compare);

    *rotationDataCount = astroidDataCount;

    return astroidData;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    int astroidCount;
    struct Astroid *astroids = getAstroids(aoc.input, &astroidCount);
    int answer = 0;

    if (astroids) {
        int maxVisible = 0;
        struct Astroid station;

        for (int i = 0; i < astroidCount; i++) {
            int visibleCount;
            struct AstroidData *visible = astroidData(astroids[i], astroids, astroidCount, true, &visibleCount);

            if (visibleCount > maxVisible) {
                station = astroids[i];
                maxVisible = visibleCount;
            }

            free(visible);
        }

        int dataCount;
        struct AstroidData *data = astroidData(station, astroids, astroidCount, false, &dataCount);

        struct Delta rotations[] = {
            { 0, -1 },
            { 1, -1 },
            { 1, 0 },
            { 1, 1 },
            { 0, 1 },
            { -1, 1 },
            { -1, 0 },
            { -1, -1 }
        };

        int rotationIndex = 0;
        int vaporizationTarget = 200;
        int vaporizedCount = 0;
        struct Astroid lastAstroid;

        while (vaporizedCount < vaporizationTarget) {
            int rotationDataCount;
            struct AstroidData **astroidData = rotationAstroidData(rotations[rotationIndex], data, dataCount, &rotationDataCount);
            float lastAngle = -1000.0;

            for (int i = 0; vaporizedCount < vaporizationTarget && i < rotationDataCount; i++) {
                if (astroidData[i]->angle == lastAngle) {
                    continue;
                }

                astroidData[i]->vaporized = true;
                
                if (++vaporizedCount == vaporizationTarget) {
                    lastAstroid = astroidData[i]->astroid;
                }

                lastAngle = astroidData[i]->angle;
            }

            if (++rotationIndex == sizeof(rotations)) {
                rotationIndex = 0;
            }

            free(astroidData);
        }

        free(astroids);
        free(data);

        answer = lastAstroid.x * 100 + lastAstroid.y;
    }

    print(aoc, INT, &answer);

    return 0;
}
