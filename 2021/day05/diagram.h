#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

struct Point {
    int x;
    int y;
};

struct Segment {
    struct Point point1;
    struct Point point2;
};

struct DiagramData {
    struct Segment *segments;
    int segmentCount;

    int **diagram;
    int maxX;
    int maxY;
};

void printSegment(struct Segment segment) {
    printf("%d,%d -> %d,%d\n", segment.point1.x, segment.point1.y, segment.point2.x, segment.point2.y);
}

void printDiagram(struct DiagramData *data) {
    for (int y = 0; y <= data->maxY; y++) {
        for (int x = 0; x <= data->maxX; x++) {
            int value = data->diagram[y][x];

            if (value > 0) {
                printf("%d ", value);
            } else {
                printf(". ");
            }
        }

        printf("\n");
    }    

    printf("\n");
}

int updateDiagram(struct DiagramData *data, bool diagonals) {
    int overlappingPoints = 0;

    for (int i = 0; i < data->segmentCount; i++) {
        struct Segment segment = data->segments[i];

        if (segment.point1.x == segment.point2.x || segment.point1.y == segment.point2.y) {
            for (int y = min(segment.point1.y, segment.point2.y); y <= max(segment.point1.y, segment.point2.y); y++) {
                for (int x = min(segment.point1.x, segment.point2.x); x <= max(segment.point1.x, segment.point2.x); x++) {
                    ++data->diagram[y][x];
                }
            }
        } else if (diagonals) {
            int y = segment.point1.y;
            int y2 = segment.point2.y;
            bool yIncreasing = y < y2;

            int x = segment.point1.x;
            int x2 = segment.point2.x;
            bool xIncreasing = x < x2;

            while ((yIncreasing ? y <= y2 : y >= y2) && (xIncreasing ? x <= x2 : x >= x2)) {
                ++data->diagram[y][x];

                x += xIncreasing ? 1 : -1;
                y += yIncreasing ? 1 : -1;
            }
        }
    }

    for (int y = 0; y <= data->maxY; y++) {
        for (int x = 0; x <= data->maxX; x++) {
            if (data->diagram[y][x] >= 2) {
                ++overlappingPoints;
            }
        }
    }    

    return overlappingPoints;
}

struct DiagramData* getDiagramData(char *input) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        struct DiagramData *data = (struct DiagramData *)calloc(1, sizeof(struct DiagramData));

        int x1, y1, x2, y2;

        while(fscanf(inputFile, "%d,%d -> %d,%d", &x1, &y1, &x2, &y2) >= 1) {
            ++data->segmentCount;

            data->maxY = max(y1, data->maxY);
            data->maxY = max(y2, data->maxY);
            data->maxX = max(x1, data->maxX);
            data->maxX = max(x2, data->maxX);
        }

        data->segments = (struct Segment *)calloc(data->segmentCount, sizeof(struct Segment));

        fseek(inputFile, 0, SEEK_SET);

        for (int i = 0; i < data->segmentCount; i++) {
            fscanf(inputFile, "%d,%d -> %d,%d", 
                &data->segments[i].point1.x, &data->segments[i].point1.y, 
                &data->segments[i].point2.x, &data->segments[i].point2.y);
        }

        fclose(inputFile);

        data->diagram = (int **)calloc(data->maxY, sizeof(int *));

        for (int i = 0; i <= data->maxY; i++) {
            data->diagram[i] = (int *)calloc(data->maxX, sizeof(int));
        }

        return data;
    }

    return NULL;
}

void freeDiagramData(struct DiagramData *data) {
    free(data->segments);

    for (int i = 0; i <= data->maxY; i++) {
        free(data->diagram[i]);
    }

    free(data->diagram);
    free(data);
}