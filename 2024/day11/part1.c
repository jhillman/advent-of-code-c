/* Day 11, part 1 = 194482 */

#include "../../aoc.h"
#include <stdlib.h>
#include <math.h>

struct Stone {
    long number;
    int digits;
    int blink;
};

struct Stones {
    int capacity;
    int count;
    struct Stone **data;
};

struct Stone *addStone(struct Stones *stones, long number, int digits, int blink) {
    if (stones->count == stones->capacity) {
        stones->capacity += 100000;
        stones->data = (struct Stone **)realloc(stones->data, stones->capacity * sizeof(struct Stone *));
    }

    struct Stone *stone = (struct Stone *)malloc(sizeof(struct Stone));
    stone->number = number;
    stone->digits = digits;
    stone->blink = blink;

    stones->data[stones->count++] = stone;

    return stone;
}

int digitCount(long number) {
    return (int)floor(log10((double)number)) + 1;
}

void blink(struct Stones *stones, int blink) {
    for (int i = 0; i < stones->count; i++) {
        struct Stone *stone = stones->data[i];

        if (stone->blink > blink) {
            continue;
        }

        stone->blink = blink + 1;

        if (stone->number == 0) {
            stone->number = 1;
        } else if (stone->digits % 2 == 0) {
            int digits = stone->digits / 2;
            long modulus = pow(10, digits);

            struct Stone *new = addStone(stones, stone->number % modulus, 0, blink + 1);
            new->digits = digitCount(new->number);

            stone->number = stone->number / modulus;
            stone->digits = digitCount(stone->number);
        } else {
            stone->number *= 2024;
            stone->digits = digitCount(stone->number);
        }
    }
}

void freeStones(struct Stones *stones) {
    for (int i = 0; i < stones->count; i++) {
        if (stones->data[i]) {
           free(stones->data[i]);
        }
    }

    free(stones->data);
    free(stones);
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        struct Stones *stones = (struct Stones *)calloc(1, sizeof(struct Stones));
        long number;

        while (!feof(inputFile)) {
            fscanf(inputFile, "%ld", &number);

            addStone(stones, number, digitCount(number), 0);
        }

        fclose(inputFile);

        for (int i = 0; i < 25; i++) {
            blink(stones, i);
        }

        answer = stones->count;

        freeStones(stones);
    }

    print(aoc, INT, &answer);

    return 0;
}
