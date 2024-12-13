#include <stdlib.h>
#include <math.h>

#define HASHSIZE 12582917L

struct Number {
    long value;
    int digits;
    long hash;
    long count;
    int blink;
};

int digitCount(long number) {
    if (number == 0) {
        return 1;
    }

    return (int)floor(log10((double)number)) + 1;
}

struct Number createNumber(long value, long count) {
    return (struct Number){ value, digitCount(value), value % HASHSIZE, count, -1 };
}

struct Numbers {
    int capacity;
    int count;
    int front;
    struct Number *data;
    struct Number *hashed;
};

struct Number addNumber(struct Numbers *numbers, long value, long count) {
    if (numbers->count == numbers->capacity) {
        numbers->capacity += 100;
        numbers->data = (struct Number *)realloc(numbers->data, numbers->capacity * sizeof(struct Number));
    }

    struct Number number = createNumber(value, count);

    numbers->data[numbers->count++] = number;

    if (numbers->hashed) {
        numbers->hashed[number.hash] = number;
    }

    return number;
}

struct Number popNumber(struct Numbers *numbers) {
    return numbers->data[numbers->front++];
}

int numberCount(struct Numbers *numbers) {
    return numbers->count - numbers->front;
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

void transform(struct Numbers *numbers, struct Transformation *transformations, struct Number input) {
    if (transformations[input.hash].outputs.count > 0) {
        return;
    }

    if (input.value == 0) {
        struct Number updated = addNumber(numbers, 1, 1);
        transformations[input.hash] = (struct Transformation){ input, { 1, { updated } } };
    } else if (input.digits % 2 == 0) {
        int digits = input.digits / 2;
        long modulus = pow(10, digits);
        long leftValue = input.value / modulus;
        long rightValue = input.value % modulus;

        if (leftValue == rightValue) {
            struct Number updated = addNumber(numbers, leftValue, 1);
            transformations[input.hash] = (struct Transformation){ input, { 2, { updated, updated } } };
        } else {
            struct Number left = addNumber(numbers, leftValue, 1);
            struct Number right = addNumber(numbers, rightValue, 1);

            transformations[input.hash] = (struct Transformation){ input, { 2, { left, right } } };
        }
    } else {
        struct Number updated = addNumber(numbers, input.value * 2024, 1);
        transformations[input.hash] = (struct Transformation){ input, { 1, { updated } } };
    }
}

long stones(char *input, int blinks) {
    FILE *inputFile = fopen(input, "r");
    long count = 0;

    if (inputFile) {
        struct Numbers *numbers = (struct Numbers *)calloc(1, sizeof(struct Numbers));
        struct Transformation *transformations = (struct Transformation *)calloc(HASHSIZE, sizeof(struct Transformation));
        long number;

        while (!feof(inputFile)) {
            fscanf(inputFile, "%ld", &number);

            addNumber(numbers, number, 1);
        }

        while (numberCount(numbers) > 0) {
            transform(numbers, transformations, popNumber(numbers));
        }

        numbers->front = 0;
        numbers->count = 0;
        numbers->hashed = (struct Number *)calloc(HASHSIZE, sizeof(struct Number));

        fseek(inputFile, 0, SEEK_SET);

        while (!feof(inputFile)) {
            fscanf(inputFile, "%ld", &number);

            addNumber(numbers, number, 1);
        }

        fclose(inputFile);

        struct Numbers *blinked = (struct Numbers *)calloc(1, sizeof(struct Numbers));
        blinked->hashed = (struct Number *)calloc(HASHSIZE, sizeof(struct Number));

        struct Numbers *swap;
        struct Transformation transformation;

        for (int blink = 0; blink < blinks; blink++) {
            for (int j = 0; j < numbers->count; j++) {
                struct Number number = numbers->hashed[numbers->data[j].hash];
                transformation = transformations[number.hash];

                for (int k = 0; k < transformation.outputs.count; k++) {
                    struct Number output = transformation.outputs.numbers[k];
                    long hash = output.hash;

                    if (blinked->hashed[hash].blink == blink + 1) {
                        blinked->hashed[hash].count += number.count;
                    } else {
                        addNumber(blinked, output.value, number.count);
                    }

                    blinked->hashed[hash].blink = blink + 1;
                }
            }

            swap = numbers;
            numbers = blinked;
            blinked = swap;
            blinked->count = 0;
        }

        for (int i = 0; i < numbers->count; i++) {
            count += numbers->hashed[numbers->data[i].hash].count;
        }

        freeNumbers(blinked);
        freeNumbers(numbers);
        free(transformations);
    }

    return count;
}
