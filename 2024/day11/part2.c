/* Day 11, part 2 = 232454623677743 */

#include "../../aoc.h"
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

struct Number {
    long value;
    int digits;
    long count;
};

int compareNumbers(struct Number *numberA, struct Number *numberB) {
    long result = numberA->value - numberB->value;

    if (result == 0) {
        return 0;
    } else if (result < 0) {
        return -1;
    } else {
        return 1;
    }    
}

int compareNumberPointers(const void *a, const void *b) {
    struct Number *numberA = *(struct Number **)a;
    struct Number *numberB = *(struct Number **)b;

    return compareNumbers(numberA, numberB);
}

struct Numbers {
    int capacity;
    int count;
    int front;
    struct Number **data;
};

int digitCount(long number) {
    if (number == 0) {
        return 1;
    }

    return (int)floor(log10((double)number)) + 1;
}

struct Number *addNumber(struct Numbers *numbers, long value, long count, bool sort) {
    if (numbers->count == numbers->capacity) {
        numbers->capacity += 100;
        numbers->data = (struct Number **)realloc(numbers->data, numbers->capacity * sizeof(struct Number *));
    }

    struct Number *number = (struct Number *)malloc(sizeof(struct Number));
    number->value = value;
    number->digits = digitCount(value);
    number->count = count;

    numbers->data[numbers->count++] = number;

    if (sort) {
        qsort(numbers->data, numbers->count, sizeof(struct Number *), compareNumberPointers);
    }

    return number;
}

struct Number popNumber(struct Numbers *numbers) {
    return *numbers->data[numbers->front++];
}

int numberCount(struct Numbers *numbers) {
    return numbers->count - numbers->front;
}

struct Number *findNumber(struct Numbers *numbers, long number, int low, int high) {
    if (numbers->count == 0 || low > high) {
        return NULL;
    }

    int middle = low + ((high - low) / 2);

    if (middle < numbers->count) {
        long result = number - numbers->data[middle]->value;

        if (result == 0) {
            return numbers->data[middle];
        } else if (result < 0) {
            return findNumber(numbers, number, low, middle - 1);
        } else {
            return findNumber(numbers, number, middle + 1, high);
        }
    }

    return NULL;
}

struct Number *getNumber(struct Numbers *numbers, long value) {
    struct Number *number = findNumber(numbers, value, 0, numbers->count);

    if (number == NULL) {
        number = addNumber(numbers, value, 0, true);
    }

    return number;
}

void freeNumbers(struct Numbers *numbers) {
    free(numbers->data);
    free(numbers);
}

struct Outputs {
    int count;
    struct Number numbers[2];
};

struct Transformation {
    struct Number input;
    struct Outputs outputs;
};

int compareTransformations(const void *a, const void *b) {
    struct Transformation *transformationA = *(struct Transformation **)a;
    struct Transformation *transformationB = *(struct Transformation **)b;

    return compareNumbers(&transformationA->input, &transformationB->input);
}

struct Transformations {
    int capacity;
    int count;
    struct Transformation **data;
};

void addTransformation(struct Transformations *transformations, struct Number input, struct Outputs outputs) {
    if (transformations->count == transformations->capacity) {
        transformations->capacity += 1000;
        transformations->data = (struct Transformation **)realloc(transformations->data, transformations->capacity * sizeof(struct Transformation *));
    }

    struct Transformation *transformation = (struct Transformation *)malloc(sizeof(struct Transformation));
    transformation->input = input;
    transformation->outputs = outputs;

    transformations->data[transformations->count++] = transformation;

    qsort(transformations->data, transformations->count, sizeof(struct Transformation *), compareTransformations);
}

struct Transformation *findTransformation(struct Transformations *transformations, long number, int low, int high) {
    if (transformations->count == 0 || low > high) {
        return NULL;
    }

    int middle = low + ((high - low) / 2);

    if (middle < transformations->count) {
        long result = number - transformations->data[middle]->input.value;

        if (result == 0) {
            return transformations->data[middle];
        } else if (result < 0) {
            return findTransformation(transformations, number, low, middle - 1);
        } else {
            return findTransformation(transformations, number, middle + 1, high);
        }
    }

    return NULL;
}

void transform(struct Numbers *numbers, struct Transformations *transformations, struct Number input) {
    struct Transformation *transformation = findTransformation(transformations, input.value, 0, transformations->count);

    if (transformation != NULL) {
        return;
    }

    if (input.value == 0) {
        struct Number updated = *addNumber(numbers, 1, 1, false);
        addTransformation(transformations, input, (struct Outputs){ 1, { updated } });
    } else if (input.digits % 2 == 0) {
        int digits = input.digits / 2;
        long modulus = pow(10, digits);
        long leftValue = input.value / modulus;
        long rightValue = input.value % modulus;

        if (leftValue == rightValue) {
            struct Number updated = *addNumber(numbers, leftValue, 1, false);
            addTransformation(transformations, input, (struct Outputs){ 2, { updated, updated } });
        } else {
            struct Number left = *addNumber(numbers, leftValue, 1, false);
            struct Number right = *addNumber(numbers, rightValue, 1, false);

            addTransformation(transformations, input, (struct Outputs){ 2, { left, right } });
        }
    } else {
        struct Number updated = *addNumber(numbers, input.value * 2024, 1, false);
        addTransformation(transformations, input, (struct Outputs){ 1, { updated } });
    }
}

void freeTransformations(struct Transformations *transformations) {
    for (int i = 0; i < transformations->count; i++) {
        free(transformations->data[i]);
    }

    free(transformations->data);
    free(transformations);
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    long answer = 0;

    if (inputFile) {
        struct Numbers *numbers = (struct Numbers *)calloc(1, sizeof(struct Numbers));
        struct Transformations *transformations = (struct Transformations *)calloc(1, sizeof(struct Transformations));
        long number;

        while (!feof(inputFile)) {
            fscanf(inputFile, "%ld", &number);

            addNumber(numbers, number, 1, false);
        }

        while (numberCount(numbers) > 0) {
            transform(numbers, transformations, popNumber(numbers));
        }

        numbers->front = 0;
        numbers->count = 0;

        fseek(inputFile, 0, SEEK_SET);

        while (!feof(inputFile)) {
            fscanf(inputFile, "%ld", &number);

            addNumber(numbers, number, 1, false);
        }

        fclose(inputFile);

        struct Numbers *blinked;
        struct Transformation *transformation;

        for (int i = 0; i < 75; i++) {
            blinked = (struct Numbers *)calloc(1, sizeof(struct Numbers));

            for (int j = 0; j < numbers->count; j++) {
                transformation = findTransformation(transformations, numbers->data[j]->value, 0, transformations->count);

                for (int k = 0; k < transformation->outputs.count; k++) {
                    getNumber(blinked, transformation->outputs.numbers[k].value)->count += numbers->data[j]->count;
                }
            }

            freeNumbers(numbers);
            numbers = blinked;
        }

        // printf("numbers: %d\n", numbers->count);

        for (int i = 0; i < numbers->count; i++) {
            answer += numbers->data[i]->count;
        }

        freeNumbers(numbers);
        freeTransformations(transformations);
    }

    print(aoc, LONG, &answer);

    return 0;
}