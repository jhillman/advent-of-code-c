#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

struct Position {
    int x;
    int y;
};

void moveHead(struct Position *head, char direction, int distance) {
    switch (direction) {
    case 'U':
        head->y -= distance;
        break;
    case 'D':
        head->y += distance;
        break;
    case 'L':
        head->x -= distance;
        break;
    case 'R':
        head->x += distance;
        break;
    }
}

void moveTail(struct Position *head, struct Position *tail) {
    int deltaX = head->x - tail->x;
    int deltaY = head->y - tail->y;

    if (abs(deltaX) > 1 || abs(deltaY) > 1) {
        if (deltaX < 0) {
            --tail->x;
        } else if (deltaX > 0) {
            ++tail->x;
        }

        if (deltaY < 0) {
            --tail->y;
        } else if (deltaY > 0) {
            ++tail->y;
        }
    }
}

int countTailPositions(char *input, int knotCount) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        struct Position *rope = (struct Position *)calloc(knotCount, sizeof(struct Position));
        struct Position *tail = &rope[knotCount - 1];

        char direction;
        int distance;
        char nl;
        int minX = 0;
        int maxX = 0;
        int minY = 0;
        int maxY = 0;

        while (fscanf(inputFile, "%c %d%c", &direction, &distance, &nl) >= 2) {
            moveHead(rope, direction, distance);

            minX = min(rope->x, minX);
            maxX = max(rope->x, maxX);
            minY = min(rope->y, minY);
            maxY = max(rope->y, maxY);
        }

        int gridWidth = maxX - minX + 1;
        int gridHeight = maxY - minY + 1;
        int gridXOffset = -minX;
        int gridYOffset = -minY;

        bool **grid = (bool **)malloc(gridHeight * sizeof(bool *));

        for (int y = 0; y < gridHeight; y++) {
            grid[y] = (bool *)calloc(gridWidth, sizeof(bool));
        }

        grid[gridYOffset][gridXOffset] = true;

        rope->x = 0;
        rope->y = 0;

        fseek(inputFile, 0, SEEK_SET);

        struct Position *knot;
        struct Position *nextKnot;

        while (fscanf(inputFile, "%c %d%c", &direction, &distance, &nl) >= 2) {
            for (int i = 0; i < distance; i++) {
                moveHead(rope, direction, 1);

                for (knot = rope, nextKnot = knot + 1; knot < tail; knot++, nextKnot++) {
                    moveTail(knot, nextKnot);
                }

                grid[tail->y + gridYOffset][tail->x + gridXOffset] = true;
            }
        }

        fclose(inputFile);

        int tailPositionCount = 0;

        for (int y = 0; y < gridHeight; y++) {
            for (int x = 0; x < gridWidth; x++) {
                if (grid[y][x]) {
                    ++tailPositionCount;
                }
            }

            free(grid[y]);
        }

        free(grid);
        free(rope);

        return tailPositionCount;
    }

    return 0;
}
