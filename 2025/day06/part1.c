/* Day 6, part 1 = 4449991244405 */

#include "../../aoc.h"
#include <stdlib.h>

struct Problem {
    long sum;
    long product;
};

struct Problems {
    int count;
    int capacity;
    struct Problem *data;
};

void addProblem(struct Problems *problems, struct Problem problem) {
    if (problems->count == problems->capacity) {
        problems->capacity += 100;
        problems->data = (struct Problem *)realloc(problems->data, problems->capacity * sizeof(struct Problem));
    }

    problems->data[problems->count++] = problem;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    long answer = 0;

    if (inputFile) {
        struct Problems *problems = (struct Problems *)calloc(1, sizeof(struct Problems));
        long number;
        char characters[8];
        int index = -1;
        long grandTotal = 0;

        while (fscanf(inputFile, "%ld%[^0-9+*]", &number, characters) >= 1) {
            if (index >= 0) {
                problems->data[index].sum += number;
                problems->data[index].product *= number;

                ++index;
            } else {
                struct Problem problem = { number, number };
                addProblem(problems, problem);
            }

            if (strchr(characters, '\n')) {
                index = 0;
            }
        }

        index = 0;

        while (!feof(inputFile)) {
            switch (fgetc(inputFile)) {
            case '+':
                grandTotal += problems->data[index++].sum;
                break;
            case '*':
                grandTotal += problems->data[index++].product;
                break;
            }
        }

        fclose(inputFile);

        free(problems->data);
        free(problems);

        answer = grandTotal;
    }

    print(aoc, LONG, &answer);

    return 0;
}
