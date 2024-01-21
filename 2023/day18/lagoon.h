#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct Digger {
    long x;
    long y;
};

struct Point {
    long x;
    long y;
};

struct Points {
    int capacity;
    int count;
    struct Point *data;
};

void addPoint(struct Points *points, struct Point point) {
    if (points->count == points->capacity) {
        points->capacity += 1000;
        points->data = (struct Point *)realloc(points->data, points->capacity * sizeof(struct Point));
    }

    points->data[points->count++] = point;
};

struct Lagoon {
    int width;
    int height;
    struct Digger digger;
    struct Points *points;
};

void dig(struct Lagoon *lagoon, char direction, long cubes) {
    struct Digger *digger = &lagoon->digger;

    switch (direction) {
    case 'U':
        if (digger->y - cubes < 0) {
            long difference = cubes - digger->y;
            lagoon->height += difference;

            for (int i = 0; i < lagoon->points->count; i++) {
                lagoon->points->data[i].y += difference;
            }

            digger->y += difference;
        }

        digger->y -= cubes;
        break;
    case 'D':
        if (digger->y + cubes > lagoon->height - 1) {
            lagoon->height += digger->y + cubes - (lagoon->height - 1);
        }

        digger->y += cubes;
        break;
    case 'L':
        if (digger->x - cubes < 0) {
            long difference = cubes - digger->x;
            lagoon->width += difference;

            for (int i = 0; i < lagoon->points->count; i++) {
                lagoon->points->data[i].x += difference;
            }

            digger->x += difference;
        }

        digger->x -= cubes;
        break;
    case 'R':
        if (digger->x + cubes > lagoon->width - 1) {
            lagoon->width += digger->x + cubes - (lagoon->width - 1);
        }

        digger->x += cubes;
        break;
    }

    addPoint(lagoon->points, (struct Point){ digger->x, digger->y });
}

void freeLagoon(struct Lagoon *lagoon) {
    free(lagoon->points->data);
    free(lagoon->points);
    free(lagoon);
}

long lagoonVolume(char *input, bool correctInstructions) {
    FILE *inputFile = fopen(input, "r");
    long volume = 0;

    if (inputFile) {
        struct Lagoon *lagoon;
        char direction;
        int cubes;
        unsigned long hexCubes;
        int directionIndex;
        char directions[] = "RDLU";
        struct Point *point1;
        struct Point *point2;
        long area = 0;
        long perimeter = 0;

        lagoon = (struct Lagoon *)calloc(1, sizeof(struct Lagoon));
        lagoon->width = 1;
        lagoon->height = 1;

        lagoon->points = (struct Points *)calloc(1, sizeof(struct Points));

        addPoint(lagoon->points, (struct Point) { 0, 0 });

        while (!feof(inputFile) && fscanf(inputFile, "%c %d (#%5lx%d)\n", &direction, &cubes, &hexCubes, &directionIndex)) {
            if (correctInstructions) {
                dig(lagoon, directions[directionIndex], (long)hexCubes);
            } else {
                dig(lagoon, direction, cubes);
            }
        }

        fclose(inputFile);

        point1 = &lagoon->points->data[0];

        for (int i = 1; i < lagoon->points->count; i++) {
            point2 = &lagoon->points->data[i];

            area += (point1->x * point2->y) - (point2->x * point1->y);
            perimeter += labs(point2->y - point1->y) + labs(point2->x - point1->x);

            point1 = point2;
        }

        volume = area / 2 + perimeter / 2 + 1;

        freeLagoon(lagoon);
    }

    return volume;
}