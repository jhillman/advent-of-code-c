#include "../../aoc.h"
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

struct XY {
    long x;
    long y;
};

struct Light {
    struct XY position;
    struct XY velocity;
};

struct Message {
    int width;
    int height;    
    bool **grid;

    int seconds;
};

struct Lights {
    int capacity;
    int count;
    struct Light *data;

    struct XY minimum;
    struct XY maximum;
    long area;

    struct Message *message;
};

void addLight(struct Lights *lights, struct Light light) {
    if (lights->count == lights->capacity) {
        lights->capacity += 100;
        lights->data = (struct Light *)realloc(lights->data, lights->capacity * sizeof(struct Light));
    }

    lights->data[lights->count++] = light;
}

void freeLights(struct Lights *lights) {
    free(lights->data);

    free(lights->message->grid);
    free(lights->message);

    free(lights);
}

void updateLights(struct Lights *lights, int multiplier) {
    lights->minimum.x = lights->minimum.y =  10000000000;
    lights->maximum.x = lights->maximum.y = -10000000000;

    for (int i = 0; i < lights->count; i++) {
        lights->data[i].position.x += (lights->data[i].velocity.x * multiplier);
        lights->data[i].position.y += (lights->data[i].velocity.y * multiplier);

        lights->minimum.x = min(lights->minimum.x, lights->data[i].position.x);
        lights->maximum.x = max(lights->maximum.x, lights->data[i].position.x);

        lights->minimum.y = min(lights->minimum.y, lights->data[i].position.y);
        lights->maximum.y = max(lights->maximum.y, lights->data[i].position.y);
    }

    lights->area = (lights->maximum.x - lights->minimum.x) * (lights->maximum.y - lights->minimum.y);
}

void findMessage(struct Lights *lights) {
    long lastArea = 10000000000000;
    bool areaShrinking = true;

    lights->message = (struct Message *)calloc(1, sizeof(struct Message));

    while (areaShrinking) {
        ++lights->message->seconds;

        updateLights(lights, 1);

        areaShrinking = lights->area < lastArea;
        lastArea = lights->area;
    }

    --lights->message->seconds;
    updateLights(lights, -1);

    lights->message->height = lights->maximum.y - lights->minimum.y + 1;
    lights->message->width = lights->maximum.x - lights->minimum.x + 1;

    lights->message->grid = (bool **)malloc(lights->message->height * sizeof(bool *));

    for (int y = 0; y < lights->message->height; y++) {
        lights->message->grid[y] = (bool *)calloc(lights->message->width, sizeof(bool));
    }

    for (int i = 0; i < lights->count; i++) {
        lights->message->grid[lights->data[i].position.y - lights->minimum.y][lights->data[i].position.x - lights->minimum.x] = true;
    }
}

void printMessage(struct Message *message) {
    for (int y = 0; y < message->height; y++) {
        for (int x = 0; x < message->width; x++) {
            fputc(message->grid[y][x] ? '#' : ' ', stdout);                                
        }

        fputs("\n", stdout);
    }
}

struct Lights *getLights(char *input) {
    FILE *inputFile = fopen(input, "r");

    struct Lights *lights = NULL;

    if (inputFile) {
        struct Light vector;

        lights = calloc(1, sizeof(struct Lights));

        while (fscanf(inputFile, "position=<%ld, %ld> velocity=<%ld, %ld>\n", &vector.position.x, &vector.position.y, &vector.velocity.x, &vector.velocity.y) == 4) {
            addLight(lights, vector);
        }

        fclose(inputFile);
    }

    return lights;
}