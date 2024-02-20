#include "../../aoc.h"
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

struct RecipeData {
    int input;
    int elfOne;
    int elfTwo;

    int capacity;
    int count;
    int *progress;

    long modulus;

    long recipes;
    int iterations;
    bool finished;
};

void setModulus(struct RecipeData *data, int digits) {
    data->modulus = pow(10, digits);
}

int addScore(struct RecipeData *data, int score) {
    if (data->count == data->capacity) {
        data->capacity += 10000000;
        data->progress = (int *)realloc(data->progress, data->capacity * sizeof(int));
    }

    data->progress[data->count++] = score;

    return score;
}

int digitCount(long number) {
    return (int)floor(log10((double)number)) + 1;
}

void updateScoring(struct RecipeData *data, int score) {
    if (data->finished) {
        return;
    }

    data->recipes *= 10;
    data->recipes += addScore(data, score);
    data->recipes %= data->modulus;

    ++data->iterations;
}

void update(struct RecipeData *data, bool (*finished)(struct RecipeData *)) {
    int sum = data->progress[data->elfOne] + data->progress[data->elfTwo];

    if (sum >= 10) {
        data->finished = finished(data);
        updateScoring(data, sum / 10);
    }

    data->finished = finished(data);
    updateScoring(data, sum % 10);

    data->elfOne = (data->elfOne + data->progress[data->elfOne] + 1) % data->count;
    data->elfTwo = (data->elfTwo + data->progress[data->elfTwo] + 1) % data->count;
}

struct RecipeData *getRecipeData(char *input) {
    FILE *inputFile = fopen(input, "r");
    
    struct RecipeData *data = NULL;

    if (inputFile) {
        data = (struct RecipeData *)calloc(1, sizeof(struct RecipeData));

        fscanf(inputFile, "%d", &data->input);
        fclose(inputFile);

        data->elfOne = 0;
        data->elfTwo = 1;

        updateScoring(data, 3);
        updateScoring(data, 7);
    }

    return data;
}

void freeRecipeData(struct RecipeData *data) {
    free(data->progress);
    free(data);
}