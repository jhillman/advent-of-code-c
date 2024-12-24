#include <stdlib.h>
#include <stdbool.h>

struct Patterns {
    int capacity;
    int count;
    char **data;
};

void addPattern(struct Patterns *patterns, char *pattern, bool copy) {
    if (patterns->count == patterns->capacity) {
        patterns->capacity += 5;
        patterns->data = (char **)realloc(patterns->data, patterns->capacity * sizeof(char *));
    }

    if (copy) {
        patterns->data[patterns->count] = (char *)malloc(strlen(pattern) + 1);
        strcpy(patterns->data[patterns->count++], pattern);
    } else {
        patterns->data[patterns->count++] = pattern;
    }
}

void clearPatterns(struct Patterns *patterns) {
    patterns->count = 0;
}

void freePatterns(struct Patterns *patterns) {
    for (int i = 0; i < patterns->count; i++) {
        free(patterns->data[i]);
    }

    free(patterns->data);
    free(patterns);
}

long towelsPossible(char *towel, int length, struct Patterns *patterns, long *cache) {
    int matchLength;
    long possible = 0;

    if (cache[length] >= 0) {
        return cache[length];
    }

    if (length == 0) {
        return 1;
    }

    for (int i = 0; i < patterns->count; i++) {
        if (strstr(towel, patterns->data[i]) == towel) {
            matchLength = strlen(patterns->data[i]);

            possible += towelsPossible(towel + matchLength, length - matchLength, patterns, cache);
        }
    }

    cache[length] = possible;

    return possible;
}

void processTowelsPossible(long possible, long *result);

void processTowelPatterns(char *input, long *result) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        struct Patterns *patterns = (struct Patterns *)calloc(1, sizeof(struct Patterns));
        struct Patterns *possible = (struct Patterns *)calloc(1, sizeof(struct Patterns));
        long *cache = (long *)malloc(64 * sizeof(long));
        char pattern[16];
        char towel[64];

        while (!feof(inputFile) && fscanf(inputFile, "%[^,\n]", pattern)) {
            addPattern(patterns, pattern, true);

            if (fgetc(inputFile) == ',') {
                fgetc(inputFile); // space
            } else {
                break;
            }
        }

        fgetc(inputFile); // \n

        while (!feof(inputFile) && fgets(towel, sizeof(towel), inputFile)) {
            towel[strcspn(towel, "\n")] = 0;

            for (int i = 0; i < patterns->count; i++) {
                if (strstr(towel, patterns->data[i])) {
                    addPattern(possible, patterns->data[i], false);
                }
            }

            memset(cache, -1, 64 * sizeof(long));
            processTowelsPossible(towelsPossible(towel, strlen(towel), possible, cache), result);

            clearPatterns(possible);
        }

        free(cache);

        fclose(inputFile);

        freePatterns(patterns);
        freePatterns(possible);
    }
}