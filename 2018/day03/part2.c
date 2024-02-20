/* Day 3, part 2 = 574 */

#include "../../aoc.h"
#include <stdlib.h>
#include <stdbool.h>

struct Point {
    int x;
    int y;
};

struct Rectangle {
    int id;
    struct Point topLeft;
    struct Point bottomRight;
    int intersections;
};

bool intersect(struct Rectangle one, struct Rectangle two) {
    if (one.bottomRight.x < two.topLeft.x || one.topLeft.x > two.bottomRight.x) {
        return false;
    }
 
    if (one.bottomRight.y < two.topLeft.y || one.topLeft.y > two.bottomRight.y) {
        return false;
    }
 
    return true;
}

struct Rectangles {
    int capacity;
    int count;
    struct Rectangle *data;
};

void addRectangle(struct Rectangles *rectangles, struct Rectangle rectangle) {
    if (rectangles->count == rectangles->capacity) {
        rectangles->capacity += 100;
        rectangles->data = (struct Rectangle *)realloc(rectangles->data, rectangles->capacity * sizeof(struct Rectangle));
    }

    rectangles->data[rectangles->count++] = rectangle;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        struct Rectangles *rectangles = (struct Rectangles *)calloc(1, sizeof(struct Rectangles));
        struct Rectangle rectangle;
        int width;
        int height;

        while (fscanf(inputFile, "#%d @ %d,%d: %dx%d\n", &rectangle.id, &rectangle.topLeft.x, &rectangle.topLeft.y, &width, &height) == 5) {
            rectangle.bottomRight = (struct Point){ rectangle.topLeft.x + width - 1, rectangle.topLeft.y + height - 1 };
            rectangle.intersections = 0;

            addRectangle(rectangles, rectangle);
        }

        fclose(inputFile);

        for (int i = 0; i < rectangles->count; i++) {
            for (int j = i + 1; j < rectangles->count; j++) {
                if (intersect(rectangles->data[i], rectangles->data[j])) {
                    ++rectangles->data[i].intersections;
                    ++rectangles->data[j].intersections;
                }
            }
        }

        for (int i = 0; !answer && i < rectangles->count; i++) {
            if (rectangles->data[i].intersections == 0) {
                answer = rectangles->data[i].id;
            }
        }

        free(rectangles->data);
        free(rectangles);
    }

    print(aoc, INT, &answer);

    return 0;
}
