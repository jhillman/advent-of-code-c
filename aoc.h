#include <stdio.h>
#include <string.h>
#include <time.h>

enum AnswerType {
    INT,
    LONG,
    LONG_LONG,
    STRING
};

struct AoC {
    char *input;
    clock_t start;
    double time;
};

struct AoC init(int argc, char **argv) {
    char *input = "input.txt";
    clock_t start = 0;
    double time = 0;

    for (int i = 1; i < argc; i++) {
        if (strstr(argv[i], ".txt")) {
            input = argv[i];
        } else if (strchr(argv[i], 't')) {
            start = clock();
        }
    }

    struct AoC aoc = { input, start, time };

    return aoc;
}

void print(struct AoC aoc, enum AnswerType type, void *answer) {
    if (aoc.start > 0) {
        aoc.time = (double)(clock() - aoc.start) / CLOCKS_PER_SEC;
    }

    switch (type) {
    case INT:
        printf("%d", *((int *)answer));
        break;
    case LONG:
        printf("%ld", *((long *)answer));
        break;
    case LONG_LONG:
        printf("%lld", *((long long *)answer));
        break;
    case STRING:
        printf("%s", (char *)answer);
        break;
    }

    if (aoc.start > 0) {
        printf(" %f", aoc.time);
    }
}