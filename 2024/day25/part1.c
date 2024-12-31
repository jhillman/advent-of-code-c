/* Day 25, part 1 = 2933 */

#include "../../aoc.h"
#include <stdlib.h>
#include <stdbool.h>

enum ValueSetType {
    LOCK,
    KEY
};

struct ValueSet {
    enum ValueSetType type;
    int values[5];
};

struct ValueSets {
    int capacity;
    int count;
    struct ValueSet *data;
};

void addValueSet(struct ValueSets *sets, struct ValueSet set) {
    if (sets->count == sets->capacity) {
        sets->capacity += 100;
        sets->data = (struct ValueSet *)realloc(sets->data, sets->capacity * sizeof(struct ValueSet));
    }

    sets->data[sets->count++] = set;
}

struct ValueSet readValueSet(FILE *inputFile) {
    char line[8];
    struct ValueSet set = { -1, { 0 } };

    fgets(line, sizeof(line), inputFile);
    line[strcspn(line, "\n")] = 0;

    if (strcmp(line, "#####") == 0) {
        set.type = LOCK;
    } else {
        set.type = KEY;
    }

    for (int i = 0; i < 7; i++) {
        line[strcspn(line, "\n")] = 0;

        for (int j = 0; j < 5; j++) {
            set.values[j] += line[j] == '#' ? 1 : 0;
        }

        if (!feof(inputFile)) {
            fgets(line, sizeof(line), inputFile);
        }
    }

    return set;
}

void freeValueSets(struct ValueSets *sets) {
    free(sets->data);
    free(sets);
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        char line[6];
        struct ValueSets *locks = (struct ValueSets *)calloc(1, sizeof(struct ValueSets));
        struct ValueSets *keys = (struct ValueSets *)calloc(1, sizeof(struct ValueSets));
        struct ValueSet set;
        struct ValueSet lock;
        struct ValueSet key;
        bool keyFits;
        int pairs = 0;

        while (!feof(inputFile)) {
            set = readValueSet(inputFile);

            switch (set.type) {
            case LOCK:
                addValueSet(locks, set);
                break;
            case KEY:
                addValueSet(keys, set);
                break;
            default:
                break;
            }
        }

        fclose(inputFile);

        for (int i = 0; i < locks->count; i++) {
            lock = locks->data[i];

            for (int j = 0; j < keys->count; j++) {
                key = keys->data[j];

                keyFits = true;

                for (int k = 0; keyFits && k < 5; k++) {
                    keyFits = lock.values[k] + key.values[k] <= 7;
                }

                if (keyFits) {
                    ++pairs;
                }
            }
        }

        answer = pairs;

        freeValueSets(locks);
        freeValueSets(keys);
    }

    print(aoc, INT, &answer);

    return 0;
}
