/* Day 3, part 2 = 312453 */

#include "../../aoc.h"
#include <stdlib.h>

struct MemoryLocation {
    int x;
    int y;
    int value;
};

struct MemoryLocations {
    int capacity;
    int count;
    struct MemoryLocation *data;
};

struct AdjacentXY {
    int x;
    int y;
};

void setMemoryValue(struct MemoryLocations *memory, int x, int y, int value) {
    if (memory->count == memory->capacity) {
        memory->capacity += 10;
        memory->data = (struct MemoryLocation *)realloc(memory->data, memory->capacity * sizeof(struct MemoryLocation));
    }

    memory->data[memory->count++] = (struct MemoryLocation){ x, y, value };
}

int getMemoryValue(struct MemoryLocations *memory, int x, int y) {
    for (int i = 0; i < memory->count; i++) {
        if (memory->data[i].x == x && memory->data[i].y == y) {
            return memory->data[i].value;
        }
    }

    return 0;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        int value;

        fscanf(inputFile, "%d", &value);
        fclose(inputFile);

        struct MemoryLocations *memory = (struct MemoryLocations *)calloc(1, sizeof(struct MemoryLocations));

        int level = 0;
        int x = 0;
        int y = 0;
        int currentValue = 1;
        struct AdjacentXY adjacents[] = {
            { -1, 0 },
            { 1, 0 },
            { 0, -1 },
            { 0, 1 },
            { -1, -1 },
            { 1, -1 },
            { -1, 1 },
            { 1, 1 }
        };

        while (currentValue < value) {
            setMemoryValue(memory, x, y, currentValue);

            if (x == level && y == level) {
                ++level;
                ++x;
            } else if (x == level && y > -level) {
                --y;
            } else if (y == -level && x > -level) {
                --x;
            } else if (x == -level && y < level) {
                ++y;
            } else if (y == level && x < level) {
                ++x;
            }

            currentValue = 0;

            for (int i = 0; i < 8; i++) {
                currentValue += getMemoryValue(memory, x + adjacents[i].x, y + adjacents[i].y);
            }
        }

        free(memory);

        answer = currentValue;
    }

    print(aoc, INT, &answer);

    return 0;
}
