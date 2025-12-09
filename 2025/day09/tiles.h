#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

struct Location {
    long x;
    long y;
};

struct Locations {
    int count;
    int capacity;
    struct Location *data;
};

void addLocation(struct Locations *locations, struct Location location) {
    if (locations->count == locations->capacity) {
        locations->capacity += 100;
        locations->data = (struct Location *)realloc(locations->data, locations->capacity * sizeof(struct Location));
    }

    locations->data[locations->count++] = location;
}

struct Line {
    struct Location start;
    struct Location end;
};

struct Lines {
    int count;
    int capacity;
    struct Line *data;
};

void addLine(struct Lines *lines, struct Location location1, struct Location location2) {
    if (lines->count == lines->capacity) {
        lines->capacity += 100;
        lines->data = (struct Line *)realloc(lines->data, lines->capacity * sizeof(struct Line));
    }

    struct Line line = { 
        min(location1.x, location2.x), min(location1.y, location2.y), 
        max(location1.x, location2.x), max(location1.y, location2.y)
    };

    lines->data[lines->count++] = line;
}

bool areaContained(struct Lines *lines, struct Location location1, struct Location location2) {
    long minX = min(location1.x, location2.x);
    long maxX = max(location1.x, location2.x);
    long minY = min(location1.y, location2.y);
    long maxY = max(location1.y, location2.y);

    for (int i = 0; i < lines->count; i++) {
        struct Line line = lines->data[i];

        if (minX < line.end.x && maxX > line.start.x &&
            minY < line.end.y && maxY > line.start.y) {
            return false;
        }
    }

    return true;
}

long largestArea(char *input, bool contained) {
    FILE *inputFile = fopen(input, "r");
    long largestArea = 0;

    if (inputFile) {
        struct Locations *locations = (struct Locations *)calloc(1, sizeof(struct Locations));
        struct Lines *lines = NULL;
        struct Location location;

        if (contained) {
            lines = (struct Lines *)calloc(1, sizeof(struct Lines));
        }

        while (fscanf(inputFile, "%ld,%ld", &location.x, &location.y) == 2) {
            if (contained && locations->count > 0) {
                addLine(lines, locations->data[locations->count - 1], location);

                if (feof(inputFile)) {
                    addLine(lines, location, *locations->data);
                }
            }

            addLocation(locations, location);
        }

        fclose(inputFile);

        for (int i = 0; i < locations->count; i++) {
            for (int j = i + 1; j < locations->count; j++) {
                struct Location location1 = locations->data[i];
                struct Location location2 = locations->data[j];
                long area = (labs(location1.x - location2.x) + 1) * (labs(location1.y - location2.y) + 1);

                if (contained && area > largestArea && areaContained(lines, location1, location2)) {
                    largestArea = area;
                } else if (!contained) {
                    largestArea = max(largestArea, area);
                }
            }
        }

        if (contained) {
            free(lines->data);
            free(lines);
        }

        free(locations->data);
        free(locations);
    }

    return largestArea;
}