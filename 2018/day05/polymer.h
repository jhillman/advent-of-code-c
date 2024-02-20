#include "../../aoc.h"
#include <stdlib.h>

char oppositePolarity(char unit) {
    return (unit < 'a') ? ('a' + unit - 'A') : ('A' + unit - 'a');
}

struct Polymer {
    int capacity;
    int length;
    char *units;
};

char peek(struct Polymer *polymer) {
    char top = '\0';

    if (polymer->length > 0) {
        top = polymer->units[polymer->length - 1];
    }

    return top;
}

void push(struct Polymer *polymer, char letter) {
    if (polymer->length == polymer->capacity) {
        polymer->capacity += 100;
        polymer->units = (char *)realloc(polymer->units, polymer->capacity * sizeof(char));
    }

    polymer->units[polymer->length++] = letter;
}

void pop(struct Polymer *polymer) {
    --polymer->length;
}

void freePolymer(struct Polymer *polymer) {
    free(polymer->units);
    free(polymer);
}

struct Lefts {
    int capacity;
    int count;
    char *data;
};

void addLeftUnit(struct Lefts *lefts, char left) {
    if (lefts->count == lefts->capacity) {
        lefts->capacity += 100;
        lefts->data = (char *)realloc(lefts->data, lefts->capacity * sizeof(char));
    }

    lefts->data[lefts->count++] = left;
}

char rightmostLeftUnit(struct Lefts *lefts) {
    return lefts->data[lefts->count - 1];
}

struct Polymer *getReactedPolymer(char *input) {
    FILE *inputFile = fopen(input, "r");
    struct Polymer *polymer = NULL;

    if (inputFile) {
        int unitCount = 0;
        char *units;
        char *unit;
        char top;

        while (!feof(inputFile) && fgetc(inputFile)) {
            ++unitCount;
        }

        fseek(inputFile, 0, SEEK_SET);

        units = (char *)malloc((unitCount + 1) * sizeof(char));

        fgets(units, unitCount + 1, inputFile);

        fclose(inputFile);

        polymer = (struct Polymer *)calloc(1, sizeof(polymer));

        unit = units;

        while (*unit) {
            top = peek(polymer); 

            if (top == oppositePolarity(*unit)) {
                pop(polymer);
            } else {
                push(polymer, *unit);
            }

            ++unit;
        }

        free(units);
    }

    return polymer;
}

// int main(int argc, char **argv) {
//     struct AoC aoc = init(argc, argv);

//     FILE *inputFile = fopen(aoc.input, "r");
//     int answer = 0;

//     if (inputFile) {
//         int length = 0;
//         struct Polymer *polymer = (struct Polymer *)calloc(1, sizeof(struct Polymer));
//         char *polymer;
//         char *unit;
//         char top;

//         while (!feof(inputFile) && fgetc(inputFile)) {
//             ++length;
//         }

//         fseek(inputFile, 0, SEEK_SET);

//         polymer = (char *)malloc((length + 1) * sizeof(char));

//         fgets(polymer, length + 1, inputFile);

//         fclose(inputFile);

//         unit = polymer;

//         while (*unit) {
//             top = peek(polymer); 

//             if (top == oppositePolarity(*unit)) {
//                 pop(polymer);
//             } else {
//                 push(polymer, *unit);
//             }

//             ++unit;
//         }

//         answer = polymer->count;

//         free(polymer);

//         free(polymer->units);
//         free(polymer);
//     }

//     print(aoc, INT, &answer);

//     return 0;
// }
