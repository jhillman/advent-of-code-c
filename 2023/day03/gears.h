#include <stdlib.h>

struct Gear {
    int x;
    int y;
    int partCount;
    int ratio;
};

struct Gears {
    int capacity;
    int count;
    struct Gear **gears;
};

struct Gear *getGear(struct Gears *gears, int x, int y) {
    for (int i = 0; i < gears->count; i++) {
        if (gears->gears[i]->x == x && gears->gears[i]->y == y) {
            return gears->gears[i];
        }
    }

    if (gears->count == gears->capacity) {
        gears->capacity += 100;

        gears->gears = (struct Gear **)realloc(gears->gears, gears->capacity * sizeof(struct Gear *));
    }

    struct Gear *gear = (struct Gear *)calloc(1, sizeof(struct Gear *));
    gear->x = x;
    gear->y = y;
    gear->ratio = 1;

    gears->gears[gears->count] = gear;

    ++gears->count;

    return gear;
}

void freeGears(struct Gears *gears) {
    for (int i = 0; i < gears->count; i++) {
        free(gears->gears[i]);
    }

    free(gears->gears);
    free(gears);
}