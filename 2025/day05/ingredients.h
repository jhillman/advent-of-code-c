#include <stdlib.h>
#include <stdbool.h>

struct Range {
    long minimum;
    long maximum;
};

struct Ranges {
    int count;
    int capacity;
    struct Range *data;
};

int compare(const void *a, const void *b) {
    struct Range *rangeA = (struct Range *)a;
    struct Range *rangeB = (struct Range *)b;

    if (rangeA->minimum < rangeB->minimum) {
        return -1;
    } else if (rangeA->minimum > rangeB->minimum) {
        return 1;
    } else if (rangeA->maximum < rangeB->maximum) {
        return -1;
    } else if (rangeA->maximum > rangeB->maximum) {
        return 1;
    } else {
        return 0;
    }
}

void addRange(struct Ranges *ranges, struct Range range) {
    if (ranges->count == ranges->capacity) {
        ranges->capacity += 100;
        ranges->data = (struct Range *)realloc(ranges->data, ranges->capacity * sizeof(struct Range));
    }

    ranges->data[ranges->count++] = range;
}

struct Ingredients {
    int count;
    int capacity;
    long *data;
};

void addIngredient(struct Ingredients *ingredients, long ingredient) {
    if (ingredients->count == ingredients->capacity) {
        ingredients->capacity += 100;
        ingredients->data = (long *)realloc(ingredients->data, ingredients->capacity * sizeof(long));
    }

    ingredients->data[ingredients->count++] = ingredient;
}

struct IngredientData {
    struct Ranges *ranges;
    struct Ingredients *ingredients;
};

struct IngredientData *getData(char *input) {
    FILE *inputFile = fopen(input, "r");
    struct IngredientData *data = NULL;

    if (inputFile) {
        data = (struct IngredientData *)calloc(1, sizeof(struct IngredientData));

        data->ranges = (struct Ranges *)calloc(1, sizeof(struct Ranges));
        data->ingredients = (struct Ingredients *)calloc(1, sizeof(struct Ingredients));
        struct Range range;
        long int previousLine;

        while (fscanf(inputFile, "%ld-%ld", &range.minimum, &range.maximum) == 2) {
            addRange(data->ranges, range);

            previousLine = ftell(inputFile);
        }

        qsort(data->ranges->data, data->ranges->count, sizeof(struct Range), compare);

        fseek(inputFile, previousLine, SEEK_SET);

        long ingredient;

        while (fscanf(inputFile, "%ld", &ingredient) >= 1) {
            addIngredient(data->ingredients, ingredient);
        }

        fclose(inputFile);
    }

    return data;
}

void freeData(struct IngredientData *data) {
    free(data->ranges->data);
    free(data->ranges);
    free(data->ingredients->data);
    free(data->ingredients);
    free(data);
}
