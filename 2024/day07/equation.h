#include <stdlib.h>
#include <stdbool.h>

struct Equation {
    int capacity;
    int length;
    long *values;
    long test;
};

void addValue(struct Equation *equation, long value) {
    if (equation->length == equation->capacity) {
        equation->capacity += 20;
        equation->values = (long *)realloc(equation->values, equation->capacity * sizeof(long));
    }

    equation->values[equation->length++] = value;
}

void freeEquation(struct Equation *equation) {
    free(equation->values);
    free(equation);
}

bool evaluate(struct Equation *equation, long value, int index, bool concatenate) {
    bool possible = false;

    if (index == equation->length) {
        if (value == equation->test) {
           possible = true;
        }
    } else {
        long next = equation->values[index++];

        if (value < equation->test && next < equation->test) {
            possible = evaluate(equation, value + next, index, concatenate);

            if (!possible) {
                possible = evaluate(equation, value * next, index, concatenate);
            }

            if (!possible && concatenate) {
                long concat = next;

                while (concat > 0) {
                    value *= 10;
                    concat /= 10;
                }

                possible = evaluate(equation, value + next, index, concatenate);
            }
        }
    }

    return possible;
}

long calibration(char *input, bool concatenate) {
    FILE *inputFile = fopen(input, "r");
    long result = 0;

    if (inputFile) {
        char line[64];
        char *value;

        while (!feof(inputFile) && fgets(line, sizeof(line), inputFile)) {
            struct Equation *equation = (struct Equation *)calloc(1, sizeof(struct Equation));
            equation->test = atol(strtok(line, ": "));

            value = strtok(NULL, ": ");

            while (value) {
                addValue(equation, atol(value));
                value = strtok(NULL, ": ");
            }

            if (evaluate(equation, *equation->values, 1, false)) {
                result += equation->test;
            } else if (concatenate && evaluate(equation, *equation->values, 1, true)) {
                result += equation->test;
            }

            freeEquation(equation);
        }

        fclose(inputFile);
    }

    return result;
}
