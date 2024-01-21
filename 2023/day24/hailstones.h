#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

enum CoordinateType {
    X,
    Y,
    Z
};

struct Coordinates {
    long long x;
    long long y;
    long long z;
};

struct Hailstone {
    struct Coordinates position;
    struct Coordinates velocity;
};

struct Hailstones {
    int capacity;
    int count;
    struct Hailstone *data;
};

void addHailstone(struct Hailstones *hailstones, struct Hailstone hailstone) {
    if (hailstones->count == hailstones->capacity) {
        hailstones->capacity += 100;
        hailstones->data = (struct Hailstone *)realloc(hailstones->data, hailstones->capacity * sizeof(struct Hailstone));
    }

    hailstones->data[hailstones->count++] = hailstone;
}

long long hailstonePosition(struct Hailstone hailstone, enum CoordinateType type) {
    switch (type) {
    case X:
        return hailstone.position.x;
    case Y:
        return hailstone.position.y;
    case Z:
        return hailstone.position.z;
    }
}

long long hailstoneVelocity(struct Hailstone hailstone, enum CoordinateType type) {
    switch (type) {
    case X:
        return hailstone.velocity.x;
    case Y:
        return hailstone.velocity.y;
    case Z:
        return hailstone.velocity.z;
    }
}

struct Set {
    bool *flags;
    int flagCapacity;
    int *values;
    int valueCapacity;
    int count;
    int minValue;
    int maxValue;
};

void setAdd(struct Set *set, int value) {
    int minValue = min(value, set->minValue);
    int maxValue = max(value, set->maxValue);
    int flagCapacity = (maxValue - minValue) + 1;

    if (flagCapacity > set->flagCapacity) {
        set->flags = (bool *)realloc(set->flags, flagCapacity * sizeof(bool));
        memset(set->flags + set->flagCapacity, false, flagCapacity - set->flagCapacity);

        set->flagCapacity = flagCapacity;
    }

    if (minValue < set->minValue) {
        int difference = minValue - set->minValue;

        for (int v = set->flagCapacity - 1; v >= 0; v--) {
            if (set->flags[v]) {
                set->flags[v] = false;
                set->flags[v - difference] = true;
            }
        }
    }

    set->minValue = minValue;
    set->maxValue = maxValue;

    if (!set->flags[value - set->minValue]) {
        if (set->count == set->valueCapacity) {
            set->valueCapacity += 10;
            set->values = (int *)realloc(set->values, set->valueCapacity * sizeof(int));
        }

        set->values[set->count++] = value;
    }

    set->flags[value - set->minValue] = true;
}

bool setContains(struct Set *set, int value) {
    if (set->minValue <= value && value <= set->maxValue) {
        return set->flags[value - set->minValue];
    }

    return false;
}

struct Set *setAnd(struct Set *first, struct Set *second) {
    struct Set *and;
    struct Set *other;

    if (first->flagCapacity > second->flagCapacity) {
        and = first;
        other = second;
    } else {
        and = second;
        other = first;
    }

    int valueCount = and->count;
    int value;

    and->count = 0;

    for (int i = 0; i < valueCount; i++) {
        value = and->values[i];

        if (setContains(other, value)) {
            and->values[and->count++] = value;
        } else {
            and->flags[value - and->minValue] = false;
        }
    }

    free(other->flags);
    free(other->values);
    free(other);

    return and;
}

int findRockVelocity(struct Hailstones *hailstones, enum CoordinateType type) {
    struct Set *possibleValues = NULL;
    struct Hailstone hailstoneOne;
    struct Hailstone hailstoneTwo;
    long long velocity;
    long long difference;

    for (int i = 0; i < hailstones->count; i++) {
        hailstoneOne = hailstones->data[i];

        for (int j = i + 1; j < hailstones->count; j++) {
            hailstoneTwo = hailstones->data[j];

            if (hailstoneVelocity(hailstoneOne, type) == hailstoneVelocity(hailstoneTwo, type) && llabs(hailstoneVelocity(hailstoneOne, type)) > 100) {
                struct Set *values = (struct Set *)calloc(1, sizeof(struct Set));
                velocity = hailstoneVelocity(hailstoneOne, type);
                difference = hailstonePosition(hailstoneTwo, type) - hailstonePosition(hailstoneOne, type);

                for (long long v = -1000; v <= 1000; v++) {
                    if (v == velocity) {
                        continue;
                    }

                    if (difference % (v - velocity) == 0) {
                        setAdd(values, v);
                    }
                }

                if (!possibleValues) {
                    possibleValues = values;
                } else {
                    possibleValues = setAnd(possibleValues, values);
                }
            }
        }
    }

    velocity = *possibleValues->values;

    free(possibleValues->flags);
    free(possibleValues->values);
    free(possibleValues);

    return velocity;
}

struct Hailstones *getHailstones(char *input) {
    FILE *inputFile = fopen(input, "r");
    struct Hailstones *hailstones = NULL;

    if (inputFile) {
        struct Hailstone hailstone;

        hailstones = (struct Hailstones *)calloc(1, sizeof(struct Hailstones));

        while (fscanf(inputFile, "%lld, %lld, %lld @ %lld, %lld, %lld",
                      &hailstone.position.x, &hailstone.position.y, &hailstone.position.z,
                      &hailstone.velocity.x, &hailstone.velocity.y, &hailstone.velocity.z) == 6) {
            addHailstone(hailstones, hailstone);
        }

        fclose(inputFile);
    }

    return hailstones;
}