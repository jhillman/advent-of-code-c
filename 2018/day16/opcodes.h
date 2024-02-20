#include "../../aoc.h"
#include "../device.h"
#include <stdbool.h>
#include <math.h>

struct Test {
    int before[4];
    int values[4];
    int after[4];
    int matchingOpcodes;
};

int matchesOneOpcode(struct Test test) {
    return test.matchingOpcodes && !(test.matchingOpcodes & (test.matchingOpcodes - 1));
}

int matchingOpcode(struct Test test) {
    return log2(test.matchingOpcodes);
}

struct Tests {
    int capacity;
    int count;
    struct Test *data;
};

void addTest(struct Tests *tests, struct Test test) {
    if (tests->count == tests->capacity) {
        tests->capacity += 10;
        tests->data = (struct Test *)realloc(tests->data, tests->capacity * sizeof(struct Test));
    }

    tests->data[tests->count++] = test;
}

void freeTests(struct Tests *tests) {
    free(tests->data);
    free(tests);
}

bool devicesMatch(int *device, int *other) {
    bool match = true;

    for (int i = 0; match && i < 4; i++) {
        match = device[i] == other[i];
    }

    return match;
}