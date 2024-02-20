/* Day 25, part 1 = 425 */

#include "../../aoc.h"
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

struct Points;

struct Point {
    int x;
    int y;
    int z;
    int w;

    struct Points *connections;
    bool inConstellation;
    bool freed;
};

int distance(struct Point *a, struct Point *b) {
    return abs(a->x - b->x) + abs(a->y - b->y) + abs(a->z - b->z) + abs(a->w - b->w);
}

struct Points {
    int capacity;
    int count;
    struct Point **data;
};

void addPoint(struct Points *points, struct Point *point) {
    if (points->count == points->capacity) {
        points->capacity += 100;
        points->data = (struct Point **)realloc(points->data, points->capacity * sizeof(struct Point *));
    }

    points->data[points->count++] = point;
}

void mark(struct Points *points, struct Point *point) {
    point->inConstellation = true;

    for (int i = 0; i < point->connections->count; i++) {
        if (!point->connections->data[i]->inConstellation) {
            mark(points, point->connections->data[i]);
        }
    }
}

void freePoints(struct Points *points, bool freeConnections) {
    if (freeConnections) {
        for (int i = 0; i < points->count; i++) {
            freePoints(points->data[i]->connections, false);

            free(points->data[i]);
        }
    }

    free(points->data);
    free(points);
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        struct Points *points = (struct Points *)calloc(1, sizeof(struct Point));
        struct Point *point;
        struct Point *a;
        struct Point *b;
        int constellations = 0;

        point = (struct Point *)calloc(1, sizeof(struct Point));

        while (fscanf(inputFile, "%d,%d,%d,%d\n", &point->x, &point->y, &point->z, &point->w) == 4) {
            point->connections = (struct Points *)calloc(1, sizeof(struct Points));

            addPoint(points, point);

            if (!feof(inputFile)) {
                point = (struct Point *)calloc(1, sizeof(struct Point));                
            }
        }

        fclose(inputFile);

        for (int i = 0; i < points->count; i++) {
            a = points->data[i];

            for (int j = i + 1; j < points->count; j++) {
                b = points->data[j];

                if (distance(a, b) <= 3) {
                    addPoint(a->connections, b);
                    addPoint(b->connections, a);
                }
            }
        }

        for (int i = 0; i < points->count; i++) {
            if (!points->data[i]->inConstellation) {
                ++constellations;

                mark(points, points->data[i]);
            }
        }

        answer = constellations;

        freePoints(points, true);
    }

    print(aoc, INT, &answer);

    return 0;
}
