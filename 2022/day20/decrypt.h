#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Number {
    long value;
    struct Number *next;
    struct Number *previous;
};

void swap(struct Number *left, struct Number *right) {
    struct Number *previous = left->previous;
    struct Number *next = right->next;

    next->previous = left;
    left->previous = right;
    left->next = next;
    right->previous = previous;
    right->next = left;
    previous->next = right;
}

long decrypt(char *input, int key, int rotationCount) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        long value;
        long numberCount = 0;
        int numberCapacity = 100;
        struct Number **numbers = (struct Number **)malloc(numberCapacity * sizeof(struct Number *));
        struct Number *first = NULL;
        struct Number *last = NULL;
        struct Number *zero;

        while (fscanf(inputFile, "%ld", &value) == 1) {
            struct Number *number = (struct Number *)calloc(1, sizeof(struct Number));
            number->value = value * key;

            if (numberCount == numberCapacity) {
                numberCapacity += 100;

                numbers = (struct Number **)realloc(numbers, numberCapacity * sizeof(struct Number *));
            }

            numbers[numberCount++] = number;

            if (!first) {
                first = number;
            } else if (value == 0) {
                zero = number;
            }

            if (last) {
                number->previous = last;
                last->next = number;
            }

            last = number;
        }

        last->next = first;
        first->previous = last;

        fclose(inputFile);

        for (int i = 0; i < rotationCount; i++) {
            for (int j = 0; j < numberCount; j++) {
                long delta = labs(numbers[j]->value) % (numberCount - 1);
                long value = numbers[j]->value;

                if (delta > numberCount / 2) {
                    delta = numberCount - delta - 1;
                    value = -value;
                }

                for (int k = 0; k < delta; k++) {
                    if (value < 0) {
                        swap(numbers[j]->previous, numbers[j]);
                    } else {
                        swap(numbers[j], numbers[j]->next);
                    }
                }
            }
        }

        long sum = 0;
        struct Number *current = zero;

        for (int i = 0; i <= 3000; i++) {
            if (i % 1000 == 0) {
                sum += current->value;
            }

            current = current->next;
        }

        for (int i = 0; i < numberCount; i++) {
            free(numbers[i]);
        }

        free(numbers);

        return sum;
    }

    return 0;
}