#include "../../aoc.h"
#include <stdlib.h>
#include <stdbool.h>

int ruleId(char *pots) {
    int id = 0;

    for (int bit = 4; bit >= 0; bit--) {
        if (pots[bit] == '#') {
            id += 1 << (4 - bit);
        }
    }

    return id;
}

struct Pots {
    int potCount;
    bool rules[32];

    char *currentGeneration;
    char *nextGeneration;
    long left;

    char *previousGeneration;
    long previousLeft;

    char *firstPlant;
    char *lastPlant;
};

void next(struct Pots *pots) {
    char *pot;
    char *nextPot;

    strcpy(pots->previousGeneration, pots->currentGeneration);
    pots->previousLeft = pots->left;

    memcpy(pots->currentGeneration + 5, pots->currentGeneration, pots->potCount);
    memset(pots->currentGeneration, '.', 5);
    memset(pots->currentGeneration + pots->potCount + 5, '.', 5);
    pots->currentGeneration[pots->potCount + 10] = '\0';

    pot = pots->currentGeneration + 2;
    nextPot = pots->nextGeneration;
    pots->firstPlant = NULL;

    for (int i = 0; i < pots->potCount + 5; i++) {
        *nextPot = pots->rules[ruleId(pot - 2)] ? '#' : '.';

        if (*nextPot == '#') {
            if (pots->firstPlant == NULL) {
                pots->firstPlant = nextPot;
            }

            pots->lastPlant = nextPot;
        }

        ++pot;
        ++nextPot;
    }

    pots->left += (pots->firstPlant - pots->nextGeneration) - 3;

    pots->potCount = pots->lastPlant - pots->firstPlant + 1;

    memcpy(pots->currentGeneration, pots->firstPlant, pots->potCount);
    pots->currentGeneration[pots->potCount] = '\0';
}

long plantSum(struct Pots *pots) {
    char *pot = pots->currentGeneration;
    long sum = 0;

    while (*pot) {
        sum += (*pot == '#' ? (pot - pots->currentGeneration) + pots->left : 0);

        ++pot;
    }

    return sum;
}

struct Pots *getPots(char *input) {
    FILE *inputFile = fopen(input, "r");
    struct Pots *pots = NULL;

    if (inputFile) {
        pots = (struct Pots *)calloc(1, sizeof(struct Pots));

        char c;
        char rule[6];

        while (true) {
            c = fgetc(inputFile);

            if (c == '.' || c == '#' || c == '\n') {
                ++pots->potCount;

                if (c == '\n') {
                    break;
                }
            }
        }

        pots->currentGeneration = (char *)malloc((pots->potCount * 2) * sizeof(char));
        pots->nextGeneration = (char *)malloc((pots->potCount * 2) * sizeof(char));
        pots->previousGeneration = (char *)malloc((pots->potCount * 2) * sizeof(char));

        fseek(inputFile, 0, SEEK_SET);
        fscanf(inputFile, "initial state: %[^\n]\n", pots->currentGeneration);

        while (fscanf(inputFile, "%[^ ] => %c\n", rule, &c) == 2) {
            pots->rules[ruleId(rule)] = (c == '#');
        }

        fclose(inputFile);

        pots->firstPlant = strchr(pots->currentGeneration, '#');
        pots->lastPlant = strrchr(pots->currentGeneration, '#');

        pots->potCount = pots->lastPlant - pots->firstPlant + 1;
    }

    return pots;
}

void freePots(struct Pots *pots) {
    free(pots->currentGeneration);
    free(pots->nextGeneration);
    free(pots->previousGeneration);

    free(pots);
}