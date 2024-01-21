#include <stdbool.h>

struct Point {
    int x;
    int y;
};

struct Line {
    struct Point point1;
    struct Point point2;
};

enum Orientation {
    HORIZONTAL,
    VERTICAL
};

struct Intersection {
    bool valid;
    struct Point point;
};

enum Orientation lineOrientation(struct Line line) {
    if (line.point1.x == line.point2.x) {
        return VERTICAL;
    } else {
        return HORIZONTAL;
    }
}

int lineLength(struct Line line) {
    int length;

    if (lineOrientation(line) == HORIZONTAL) {
        int x1 = line.point1.x;
        int x2 = line.point2.x;

        if (x2 > x1) {
            return x2 - x1;
        } else {
            return x1 - x2;
        }
    } else {
        int y1 = line.point1.y;
        int y2 = line.point2.y;

        if (y2 > y1) {
            return y2 - y1;
        } else {
            return y1 - y2;
        }
    }
}

int pointInsideLine(struct Point point, struct Line line) {
    return (point.x >= line.point1.x && point.x <= line.point2.x || point.x >= line.point2.x && point.x <= line.point1.x)
               && (point.y >= line.point1.y && point.y <= line.point2.y || point.y >= line.point2.y && point.y <= line.point1.y);
}

struct Intersection findIntersection(struct Line line1, struct Line line2) {
    struct Intersection intersection = { 0, { 0, 0 } };
    enum Orientation line1Orientation = lineOrientation(line1);
    enum Orientation line2Orientation = lineOrientation(line2);

    if (line1Orientation != line2Orientation) {
        struct Point point;

        if (line1Orientation == HORIZONTAL) {
            point.x = line2.point1.x;
            point.y = line1.point1.y;
        } else {
            point.x = line1.point1.x;
            point.y = line2.point1.y;
        }

        if (pointInsideLine(point, line1) && pointInsideLine(point, line2)) {
            intersection.valid = true;
            intersection.point = point;
        }
    }

    return intersection;
}

struct WireData {
    int path1Length;
    struct Point *wire1Path;
    int path2Length;
    struct Point *wire2Path;
};

struct WireData readInput(char *input) {
    FILE *inputFile = fopen(input, "r");

    struct WireData data;
    data.path1Length = 0;
    data.path2Length = 0;

    if (inputFile) {
        char ch;
        int path1 = 1;

        while (!feof(inputFile)) {
            ch = fgetc(inputFile);

            if (ch == ',') {
                if (path1) {
                    ++data.path1Length;
                } else {
                    ++data.path2Length;
                }
            } else if (ch == '\n') {
                if (data.path1Length > 0) {
                    ++data.path1Length;
                }

                path1 = 0;
            }
        }

        if (data.path2Length > 0) {
            ++data.path2Length;
        }

        path1 = 1;

        fseek(inputFile, 0, SEEK_SET);

        ++data.path1Length;
        ++data.path2Length;

        data.wire1Path = (struct Point *)calloc(data.path1Length, sizeof(struct Point));
        data.wire2Path = (struct Point *)calloc(data.path2Length, sizeof(struct Point));

        int pathPosition = 0;
        struct Point *wirePath = data.wire1Path;
        char direction;
        int value;

        struct Point lastPoint = { 0, 0 };
        wirePath[pathPosition++] = lastPoint;

        while (fscanf(inputFile, "%c%d%c", &direction, &value, &ch) >= 2) {
            int x = lastPoint.x;
            int y = lastPoint.y;
            struct Point point;

            switch (direction) {
                case 'L':
                    point = (struct Point) { x - value, y };
                    break;
                case 'R':
                    point = (struct Point) { x + value, y };
                    break;
                case 'U':
                    point = (struct Point) { x, y + value };
                    break;
                case 'D':
                    point = (struct Point) { x, y - value };
                    break;
            }

            wirePath[pathPosition++] = point;

            lastPoint = point;

            if (ch == '\n') {
                pathPosition = 0;
                wirePath = data.wire2Path;

                lastPoint = (struct Point) { 0, 0 };
                wirePath[pathPosition++] = lastPoint;
            }
        }

        fclose(inputFile);
    }

    return data;
}

void freeData(struct WireData data) {
    free(data.wire1Path);
    free(data.wire2Path);
}