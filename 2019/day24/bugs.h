#include <stdlib.h>
#include <stdbool.h>

struct Layouts {
    int capacity;
    int count;
    int *data;
};

int compare(const void *a, const void *b) {
    return *((int *)a) - *((int *)b);
}

void addLayout(struct Layouts *layouts, int layout, bool sort) {
    if (layouts->count == layouts->capacity) {
        layouts->capacity += 1000;
        layouts->data = (int *)realloc(layouts->data, layouts->capacity * sizeof(int));
    }

    layouts->data[layouts->count++] = layout;

    if (sort) {
        qsort(layouts->data, layouts->count, sizeof(int), compare);
    }
}

int findLayout(struct Layouts *layouts, int layout, int low, int high) {
    if (layouts->count == 0 || low > high) {
        return -1;
    } 

    int middle = low + ((high - low) / 2);

    if (layouts->data[middle] == layout) {
        return middle;
    } else if (layouts->data[middle] > layout) {
        return findLayout(layouts, layout, low, middle - 1);
    } else {
        return findLayout(layouts, layout, middle + 1, high);
    }
}

bool layoutSeen(struct Layouts *layouts, int layout) {
    return findLayout(layouts, layout, 0, layouts->count) != -1;
}

void freeLayouts(struct Layouts *layouts) {
    free(layouts->data);
    free(layouts);
}

bool hasBug(int layout, int x, int y) {
    if (!(x >= 0 && x < 5 && y >= 0 && y < 5)) {
        return false;
    }

    return ((layout >> (y * 5 + x)) & 1) == 1;
}

void printLayout(int layout) {
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
            printf("%c", hasBug(layout, x, y) ? '#' : '.');
        }

        printf("\n");
    }    
}

enum CalculationType {
	BIODIVERSITY_RATING,
	BUG_COUNT
};

int calculate(char *input, enum CalculationType type) {
    FILE *inputFile = fopen(input, "r");
    int calculation = 0;

    if (inputFile) {
        char c;
        int bit = 1;
        int layout = 0;

        while (!feof(inputFile)) {
            c = fgetc(inputFile);

            if (c == '#') {
                layout += bit;
            }

            if (c != '\n') {
                bit <<= 1;
            }
        }

        fclose(inputFile);

        struct Layouts *visited = (struct Layouts *)calloc(1, sizeof(struct Layouts));
        struct Layouts *layouts = (struct Layouts *)calloc(1, sizeof(struct Layouts));
        struct Layouts *previousLayouts = (struct Layouts *)calloc(1, sizeof(struct Layouts));

        int xDeltas[] = {  0, 1, 0, -1 };
        int yDeltas[] = { -1, 0, 1,  0 };
        int loops = 0;

        addLayout(layouts, layout, false);

        while ((type == BIODIVERSITY_RATING && !layoutSeen(visited, layout)) || (type == BUG_COUNT && loops < 200)) {
            addLayout(visited, layout, true);

            addLayout(previousLayouts, 0, false);

            memcpy(previousLayouts->data + 1, layouts->data, layouts->count * sizeof(int));
            previousLayouts->count += layouts->count;

            addLayout(previousLayouts, 0, false);

            layouts->count = 0;

            for (int i = 0; i < previousLayouts->count; i++) {
                layout = 0;

                for (int y = 0; y < 5; y++) {
                    for (int x = 0; x < 5; x++) {
                        if (type == BUG_COUNT && x == 2 && y == 2) {
                            continue;
                        }

                        int bugCount = 0;

                        for (int j = 0; j < 4; j++) {
                            int dx = x + xDeltas[j];
                            int dy = y + yDeltas[j];

                            if (type == BUG_COUNT) {
                                if (dx == 2 && dy == 2) {
                                    if (y == 2) {
                                        for (int dy = 0; dy < 5; dy++) {
                                            if ((x == 1 && hasBug(previousLayouts->data[i + 1], 0, dy)) || 
                                                (x == 3 && hasBug(previousLayouts->data[i + 1], 4, dy))) {
                                                ++bugCount;
                                            }
                                        }
                                    } else {
                                        for (int dx = 0; dx < 5; dx++) {
                                            if ((y == 1 && hasBug(previousLayouts->data[i + 1], dx, 0)) || 
                                                (y == 3 && hasBug(previousLayouts->data[i + 1], dx, 4))) {
                                                ++bugCount;
                                            }
                                        }
                                    }
                                } else if ((dx < 0 && hasBug(previousLayouts->data[i - 1], 1, 2))) {
                                    ++bugCount;
                                } else if ((dy < 0 && hasBug(previousLayouts->data[i - 1], 2, 1))) {
                                    ++bugCount;
                                } else if ((dx >= 5 && hasBug(previousLayouts->data[i - 1], 3, 2))) {
                                    ++bugCount;
                                } else if ((dy >= 5 && hasBug(previousLayouts->data[i - 1], 2, 3))) {
                                    ++bugCount;
                                } else if (hasBug(previousLayouts->data[i], dx, dy)) {
                                    ++bugCount;
                                }
                            } else if (hasBug(previousLayouts->data[i], dx, dy)) {
                                ++bugCount;
                            }
                        }

                        if (!hasBug(previousLayouts->data[i], x, y)) {
                            if (bugCount == 1 || bugCount == 2) {
                                layout |= (1 << (y * 5 + x));
                            }
                        } else {
                            if (bugCount == 1) {
                                layout |= (1 << (y * 5 + x));
                            }
                        }
                    }
                }

                addLayout(layouts, layout, false);
            }

            previousLayouts->count = 0;

            layout = layouts->data[layouts->count / 2];
            ++loops;
        }

        if (type == BUG_COUNT) {
            for (int i = 0; i < layouts->count; i++) {
                for (int y = 0; y < 5; y++) {
                    for (int x = 0; x < 5; x++) {
                        if (!(y == 2 && x == 2) && hasBug(layouts->data[i], x, y)) {
                            ++calculation;
                        }
                    }
                }
            }
        } else {
            calculation = layout;
        }

        freeLayouts(visited);
        freeLayouts(layouts);
        freeLayouts(previousLayouts);
    }

    return calculation;
}