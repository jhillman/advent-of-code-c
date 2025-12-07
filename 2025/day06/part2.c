/* Day 6, part 2 = 9348430857627 */

#include "../../aoc.h"
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define max(a,b) ((a) > (b) ? (a) : (b))

struct Number {
    long value;
    int digits;
    int indentation;
};

struct Numbers {
    int count;
    int capacity;
    struct Number *data;
};

enum Operation {
    ADD,
    MULTIPLY
};

struct Problem {
    enum Operation operation;
    struct Numbers *numbers;
    int width;
    char format[8];
    int maxDigits;
};

struct Problems {
    int count;
    int capacity;
    struct Problem *data;
};

int digitCount(long number) {
    int count = 0;

    while (number > 0) {
      number /= 10;
      count++;
    }

    return count;
}

void addProblem(struct Problems *problems, enum Operation operation, int width) {
    if (problems->count == problems->capacity) {
        problems->capacity += 100;
        problems->data = (struct Problem *)realloc(problems->data, problems->capacity * sizeof(struct Problem));
    }
    
    struct Problem problem = {
        operation,
        calloc(1, sizeof(struct Numbers)),
        width + 1,
        0, 0
    };

    sprintf(problem.format, "%%%dc", width + 1);
    
    problems->data[problems->count++] = problem;
}

struct Number *addValue(struct Numbers *numbers, long value) {
    if (numbers->count == numbers->capacity) {
        numbers->capacity += 100;
        numbers->data = (struct Number *)realloc(numbers->data, numbers->capacity * sizeof(struct Number));
    }

    struct Number number = { value };

    numbers->data[numbers->count] = number;

    return &numbers->data[numbers->count++];
}

void addNumber(struct Problem *problem, long value, int indentation) {
    struct Number *number = addValue(problem->numbers, value);
    number->indentation = indentation;
    number->digits = digitCount(value);

    problem->maxDigits = max(problem->maxDigits, number->digits);
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    long answer = 0;

    if (inputFile) {
        struct Problems *problems = (struct Problems *)calloc(1, sizeof(struct Problems));
        int operandCount = 0;
        char character;
        bool problemAdded = false;
        char operation = 0;
        int width = 0;
        long number;
        char characters[8];
        char *charPtr;
        int indentation;
        long grandTotal = 0;

        while (fscanf(inputFile, "%*ld%[^0-9+*]", characters)) {
            if (strchr(characters, '\n')) {
                ++operandCount;
            }
        }

        while (!feof(inputFile)) {
            character = fgetc(inputFile);

            if (problemAdded) {
                width = 0;
                problemAdded = false;
            }

            switch (character) {
            case '+':
            case '*':
                if (operation) {
                    addProblem(problems, operation == '+' ? ADD : MULTIPLY, width);

                    problemAdded = true;
                }

                operation = character;
                break;
            default:
                ++width;
                break;
            }
        }

        if (operation) {
            addProblem(problems, operation == '+' ? ADD : MULTIPLY, width);
        }

        fseek(inputFile, 0, SEEK_SET);

        for (int i = 0; i < operandCount; i++) {
            for (int j = 0; j < problems->count; j++) {
                fscanf(inputFile, problems->data[j].format, characters);
                characters[problems->data[j].width] = '\0';

                indentation = 0;
                charPtr = characters;

                while (isspace(*charPtr)) {
                    ++indentation;
                    ++charPtr;
                }

                addNumber(&problems->data[j], atol(charPtr), indentation);
            }
        }

        fclose(inputFile);

        for (int i = 0; i < problems->count; i++) {
            struct Numbers *numbers = problems->data[i].numbers;
            struct Numbers *rtlNumbers = (struct Numbers *)calloc(1, sizeof(struct Numbers));
            long digit = problems->data[i].maxDigits;

            while (digit > 0) {
                long number = 0;

                for (int j = 0; j < numbers->count; j++) {
                    if (numbers->data[j].value && numbers->data[j].digits + numbers->data[j].indentation == digit) {
                        number *= 10;
                        number += numbers->data[j].value % 10;

                        numbers->data[j].value /= 10;
                        --numbers->data[j].digits;
                    }
                }

                addValue(rtlNumbers, number);

                --digit;
            }

            for (int j = 0; j < rtlNumbers->count; j++) {
                if (j == 0) {
                    answer = rtlNumbers->data[j].value;
                } else {
                    switch (problems->data[i].operation) {
                    case ADD:
                        answer += rtlNumbers->data[j].value;
                        break;
                    case MULTIPLY:
                        answer *= rtlNumbers->data[j].value;
                        break;
                    }
                }
            }

            free(rtlNumbers->data);
            free(rtlNumbers);

            grandTotal += answer;
        }

        for (int i = 0; i < problems->count; i++) {
            free(problems->data[i].numbers->data);
            free(problems->data[i].numbers);
        }

        free(problems->data);
        free(problems);

        answer = grandTotal;
    }

    print(aoc, LONG, &answer);

    return 0;
}
