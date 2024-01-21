#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

struct Location {
    int x;
    int y;
};

struct Locations {
    int capacity;
    int count;
    struct Location *data;
};

void addLocation(struct Locations *locations, struct Location location) {
    if (locations->count == locations->capacity) {
        locations->capacity += 10;
        locations->data = (struct Location *)realloc(locations->data, locations->capacity * sizeof(struct Location));
    }

    locations->data[locations->count++] = location;
}

long galaxyDistanceSum(char *input, long expansion) {
    FILE *inputFile = fopen(input, "r");
    long sum = 0;

    if (inputFile) {
        char c;
        int width = 0;
        int height = 0;
        struct Locations *locations = (struct Locations *)calloc(1, sizeof(struct Locations));
        bool *galaxyRows;
        bool *galaxyColumns;
        long answer = 0;

        while (!feof(inputFile)) {
            c = fgetc(inputFile);

            if (c == '\n' || feof(inputFile)) {
                ++height;
            }

            if (!height) {
                ++width;
            }
        }

        fseek(inputFile, 0, SEEK_SET);

        galaxyRows = (bool *)calloc(height, sizeof(bool));
        galaxyColumns = (bool *)calloc(width, sizeof(bool));

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                c = fgetc(inputFile);

                if (c == '#') {
                    addLocation(locations, (struct Location){ x, y });
                }

                galaxyRows[y] = galaxyRows[y] || c == '#';
                galaxyColumns[x] = galaxyColumns[x] || c == '#';
            }

            fgetc(inputFile); // \n
        }

        fclose(inputFile);

        for (int i = 0; i < locations->count; i++) {
            for (int j = i + 1; j < locations->count; j++) {
                long x1 = locations->data[i].x;
                long y1 = locations->data[i].y;
                long x2 = locations->data[j].x;
                long y2 = locations->data[j].y;
                long expandedX = 0;
                long expandedY = 0;

                for (int x = min(x1, x2); x <= max(x1, x2); x++) {
                    if (!galaxyColumns[x]) {
                        expandedX += (expansion - 1);
                    }
                }

                for (int y = min(y1, y2); y <= max(y1, y2); y++) {
                    if (!galaxyRows[y]) {
                        expandedY += (expansion - 1);
                    }
                }

                sum += labs(y2 - y1) + expandedY + labs(x2 - x1) + expandedX;
            }
        }

        free(galaxyRows);
        free(galaxyColumns);

        free(locations->data);
        free(locations);
    }

    return sum;
}