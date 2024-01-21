#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct PolymerPair {
    char left;
    char right;
    char insertion;
    long count;
};

struct PolymerData {
    struct PolymerPair *pairs;
    int pairCount;

    long elements[26];
};

void addPair(struct PolymerData *data, char left, char right, char insertion) {
    ++data->pairCount;
    data->pairs = (struct PolymerPair *)realloc(data->pairs, data->pairCount * sizeof(struct PolymerPair));

    data->pairs[data->pairCount - 1].left = left;
    data->pairs[data->pairCount - 1].right = right;
    data->pairs[data->pairCount - 1].insertion = insertion;
}

void increasePairCount(struct PolymerData *data, char left, char right, long count) {
    for (int i = 0; i < data->pairCount; i++) {
        struct PolymerPair pair = data->pairs[i];

        if (pair.left == left && pair.right == right) {
            data->pairs[i].count += count;
            break;
        }
    }
}

long performSteps(struct PolymerData *data, int steps) {
    struct PolymerPair *pairs = (struct PolymerPair *)calloc(data->pairCount, sizeof(struct PolymerPair));

    for (int i = 0; i < steps; i++) {
        memcpy(pairs, data->pairs, data->pairCount * sizeof(struct PolymerPair));

        for (int j = 0; j < data->pairCount; j++) {
            if (pairs[j].count > 0) {
                data->elements[pairs[j].insertion - 'A'] += pairs[j].count;

                data->pairs[j].count -= pairs[j].count;

                increasePairCount(data, pairs[j].left, pairs[j].insertion, pairs[j].count);
                increasePairCount(data, pairs[j].insertion, pairs[j].right, pairs[j].count);
            }
        }
    }

    free(pairs);

    long elementCount;
    long maxElementCount = 0;
    long minElementCount = 10000000000000;

    for (int i = 0; i < 26; i++) {
        if ((elementCount = data->elements[i])) {
            if (elementCount < minElementCount) {
                minElementCount = elementCount;
            }

            if (elementCount > maxElementCount) {
                maxElementCount = elementCount;
            }
        }
    }

    return maxElementCount - minElementCount;
}

struct PolymerData *getPolymerData(char *input) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        char polymerTemplate[32];
        char *element;
        char left;
        char right;
        char insertion;
        char c;

        struct PolymerData *data = (struct PolymerData *)calloc(1, sizeof(struct PolymerData));

        fgets(polymerTemplate, sizeof(polymerTemplate), inputFile);

        // \n
        fgetc(inputFile);

        while (fscanf(inputFile, "%c%c -> %c%c", &left, &right, &insertion, &c) > 0) {
            addPair(data, left, right, insertion);
        }

        fclose(inputFile);

        element = polymerTemplate;

        left = '\0';
        right = '\0';

        while (*element && *element != '\n') {
            ++data->elements[*element - 'A'];

            right = *element;

            if (left && right) {
                increasePairCount(data, left, right, 1);
            }

            left = right;

            ++element;
        }

        return data;
    }

    return NULL;
}

void freePolymerData(struct PolymerData *data) {
    free(data->pairs);
    free(data);
}
