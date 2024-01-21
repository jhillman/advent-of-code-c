#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Position {
    int x;
    int y;
};

struct Stepper {
    struct Position last;
    struct Position current;
};

struct Tiles {
    int width;
    int height;
    char **data;
    struct Stepper stepper1;
    struct Stepper stepper2;
};

struct Stepper updateStepper(struct Tiles *tiles, struct Stepper stepper) {
    struct Position change = { 0, 0 };

    switch (tiles->data[stepper.current.y][stepper.current.x]) {
    case '|':
        if (stepper.current.y > stepper.last.y) {
            ++change.y;
        } else {
            --change.y;
        }
        break;
    case '-':
        if (stepper.current.x > stepper.last.x) {
            ++change.x;
        } else {
            --change.x;
        }
        break;
    case 'L':
        if (stepper.current.y > stepper.last.y) {
            ++change.x;
        } else {
            --change.y;
        }
        break;
    case 'J':
        if (stepper.current.y > stepper.last.y) {
            --change.x;
        } else {
            --change.y;
        }
        break;
    case '7':
        if (stepper.current.y < stepper.last.y) {
            --change.x;
        } else {
            ++change.y;
        }
        break;
    case 'F':
        if (stepper.current.y < stepper.last.y) {
            ++change.x;
        } else {
            ++change.y;
        }
        break;
    }

    stepper.last = stepper.current;
    stepper.current = (struct Position){ stepper.last.x + change.x, stepper.last.y + change.y };

    return stepper;
}

struct Tiles * getTiles(char *input) {
    FILE *inputFile = fopen(input, "r");
    struct Tiles *tiles = NULL;

    if (inputFile) {
        char c;
        int xDeltas[] = { 1, 0, -1, 0 };
        int yDeltas[] = { 0, 1, 0, -1 };
        char pipeOptions[4][4] =      { "-7J", "|JL", "-LF", "|7F" };
        char pipeStartOptions[4][4] = { "-LF", "|7F", "-J7", "|JL" };
        char stepper1StartOptions[4];
        char stepper2StartOptions[4];
        struct Stepper *stepper = NULL;
        char *stepperStartOptions;
        char *pipe = NULL;
        int answer = 0;

        tiles = (struct Tiles *)calloc(1, sizeof(struct Tiles));

        while (!feof(inputFile)) {
            c = fgetc(inputFile);

            if (c == '\n' || feof(inputFile)) {
                ++tiles->height;
            }

            if (!tiles->height) {
                ++tiles->width;
            }
        }

        fseek(inputFile, 0, SEEK_SET);

        tiles->data = (char **)malloc(tiles->height * sizeof(char *));

        for (int y = 0; y < tiles->height; y++) {
            tiles->data[y] = (char *)malloc(tiles->width * sizeof(char));

            for (int x = 0; x < tiles->width; x++) {
                tiles->data[y][x] = fgetc(inputFile);

                if (tiles->data[y][x] == 'S') {
                    tiles->stepper1.current.x = tiles->stepper2.current.x = x;
                    tiles->stepper1.current.y = tiles->stepper2.current.y = y;
                }
            }

            fgetc(inputFile); // \n
        }

        fclose(inputFile);

        for (int i = 0; i < 4; i++) {
            int x = tiles->stepper1.current.x + xDeltas[i];
            int y = tiles->stepper1.current.y + yDeltas[i];

            char *pipe = strchr(pipeOptions[i], tiles->data[y][x]);

            if (pipe) {
                if (!stepper) {
                    stepper = &tiles->stepper1;
                    stepperStartOptions = stepper1StartOptions;
                } else {
                    stepper = &tiles->stepper2;
                    stepperStartOptions = stepper2StartOptions;
                }

                stepper->last.x = x;
                stepper->last.y = y;

                strcpy(stepperStartOptions, pipeStartOptions[i]);
            }
        }

        for (int i = 0; !pipe && i < 3; i++) {
            pipe = strchr(stepper2StartOptions, stepper1StartOptions[i]);

            if (pipe) {
                tiles->data[tiles->stepper1.current.y][tiles->stepper1.current.x] = *pipe;
            }
        }
    }

    return tiles;
}

void freeTiles(struct Tiles *tiles) {
    for (int y = 0; y < tiles->height; y++) {
        free(tiles->data[y]);
    }

    free(tiles->data);
    free(tiles);
}