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

void evaluate(struct Equation *equation, long value, int index, bool concatenate, bool *possible) {
    if (index == equation->length) {
        if (value == equation->test) {
           *possible = true;
        }
    } else {
        long next = equation->values[index++];

        if (value > equation->test || next > equation->test) {
            return;
        }

        evaluate(equation, value + next, index, concatenate, possible);

        if (equation->test % value == 0) {
            evaluate(equation, value * next, index, concatenate, possible);
        }

        if (concatenate) {
            long concat = next;

            while (concat > 0) {
                value *= 10;
                concat /= 10;
            }

            evaluate(equation, value + next, index, concatenate, possible);
        }
    }
}

long calibration(char *input, bool concatenate) {
    FILE *inputFile = fopen(input, "r");
    long result = 0;

    if (inputFile) {
        char line[64];
        long test;
        char *value;
        bool possible;

        while (!feof(inputFile) && fgets(line, sizeof(line), inputFile)) {
            struct Equation *equation = (struct Equation *)calloc(1, sizeof(struct Equation));
            test = atol(strtok(line, ": "));

            equation->test = test;

            value = strtok(NULL, ": ");

            while (value) {
                addValue(equation, atol(value));
                value = strtok(NULL, ": ");
            }

            possible = false;

            evaluate(equation, *equation->values, 1, concatenate, &possible);
            freeEquation(equation);

            if (possible) {
                result += test;
            }
        }

        fclose(inputFile);
    }

    return result;
}
