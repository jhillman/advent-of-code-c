#include <stdio.h>
#include <stdlib.h>

#define DOT 1

enum Axis {
    X,
    Y
};

struct Fold {
    enum Axis axis;
    int value;
};

struct DotsData {
    int width;
    int height;
    int originalHeight;

    int **dots;
    int dotCount;

    struct Fold *folds;
    int foldCount;
};

int foldDots(struct DotsData *data, struct Fold fold) {
    int visibleDots = 0;

    switch (fold.axis) {
        case X:
            for (int y = 0; y < data->height; y++) {
                for (int x = fold.value + 1; x < data->width; x++) {
                    data->dots[y][fold.value - (x - fold.value)] |= data->dots[y][x];
                    visibleDots += data->dots[y][fold.value - (x - fold.value)];
                }
            }

            data->width = fold.value;
            break;
        case Y:
            for (int y = fold.value + 1; y < data->height; y++) {
                for (int x = 0; x < data->width; x++) {
                    data->dots[fold.value - (y - fold.value)][x] |= data->dots[y][x];
                    visibleDots += data->dots[fold.value - (y - fold.value)][x];
                }
            }

            data->height = fold.value;
            break;
    }

    return visibleDots;
}

void printDots(struct DotsData *data) {
    for (int y = 0; y < data->height; y++) {
        for (int x = 0; x < data->width; x++) {
            printf(data->dots[y][x] ? "#" : " ");
        }

        printf("\n");
    }
}

struct DotsData *getDotsData(char *input) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        struct DotsData *data = (struct DotsData *)calloc(1, sizeof(struct DotsData));
        int x;
        int y;
        char axis;
        int value;
        char c;

        while (fscanf(inputFile, "%d,%d", &x, &y) > 0) {
            if (x > data->width) {
                data->width = x;
            }

            if (y > data->height) {
                data->height = y;
            }
        }

        ++data->width;
        ++data->height;

        data->originalHeight = data->height;

        while (fscanf(inputFile, "fold along %c=%d%c", &axis, &value, &c) > 0) {
            ++data->foldCount;
        }

        fseek(inputFile, 0, SEEK_SET);

        data->dots = (int **)calloc(data->height, sizeof(int *));

        for (int y = 0; y < data->height; y++) {
            data->dots[y] = (int *)calloc(data->width, sizeof(int));
        }

        while (fscanf(inputFile, "%d,%d", &x, &y) > 0) {
            data->dots[y][x] = DOT;
        }

        data->folds = (struct Fold *)calloc(data->foldCount, sizeof(struct Fold));

        for (int i = 0; i < data->foldCount; i++) {
            fscanf(inputFile, "fold along %c=%d%c", &axis, &value, &c);

            data->folds[i].axis = axis == 'x' ? X : Y;
            data->folds[i].value = value;
        }

        fclose(inputFile);

        return data;
    }

    return NULL;
}

void freeDotsData(struct DotsData *data) {
    for (int y = 0; y < data->originalHeight; y++) {
        free(data->dots[y]);
    }

    free(data->dots);
    free(data->folds);
    free(data);
}