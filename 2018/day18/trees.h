#include "../../aoc.h"
#include <stdlib.h>
#include <math.h>

struct CollectionData {
    int width;
    int height;
    char **area;
    char **update;

    int treeAcres;
    int lumberyardAcres;
};

struct CollectionData *getCollectionData(char *input) {
    FILE *inputFile = fopen(input, "r");
    struct CollectionData *data = NULL;

    if (inputFile) {
        data = (struct CollectionData *)calloc(1, sizeof(struct CollectionData));

        while (!feof(inputFile)) {
            if (fgetc(inputFile) == '\n' || feof(inputFile)) {
                ++data->height;
            }

            if (!data->height) {
                ++data->width;
            }
        }

        fseek(inputFile, 0, SEEK_SET);

        data->area = (char **)malloc(data->height * sizeof(char *));
        data->update = (char **)malloc(data->height * sizeof(char *));

        for (int y = 0; y < data->height; y++) {
            data->area[y] = (char *)malloc(data->width * sizeof(char));
            data->update[y] = (char *)malloc(data->width * sizeof(char));

            for (int x = 0; x < data->width; x++) {
                data->area[y][x] = fgetc(inputFile);
            }

            fgetc(inputFile); // \n
        }

        fclose(inputFile);
    }

    return data;
}

void freeCollectionData(struct CollectionData *data) {
    for (int y = 0; y < data->height; y++) {
        free(data->area[y]);
        free(data->update[y]);
    }

    free(data->area);
    free(data->update);

    free(data);
}

struct AdjacentXY {
    int x;
    int y;
};

struct UpdateHash {
    long long hash;
    int iteration;
};

int compare(const void *a, const void *b) {
    long long result = ((struct UpdateHash *)a)->hash - ((struct UpdateHash *)b)->hash;

    if (result == 0) {
        return 0;
    } else if (result < 0) {
        return -1;
    } else {
        return 1;
    }
}

struct UpdateHashes {
    int capacity;
    int count;
    struct UpdateHash *data;
};

void addHash(struct UpdateHashes *hashes, struct UpdateHash hash) {
    if (hashes->count == hashes->capacity) {
        hashes->capacity += 10;
        hashes->data = (struct UpdateHash *)realloc(hashes->data, hashes->capacity * sizeof(struct UpdateHash));
    }

    hashes->data[hashes->count++] = hash;

    qsort(hashes->data, hashes->count, sizeof(struct UpdateHash), compare);
}

struct UpdateHash *findHash(struct UpdateHashes *hashes, long long hash, int low, int high) {
    if (hashes->count == 0 || low > high) {
        return NULL;
    }

    int middle = low + ((high - low) / 2);

    if (hashes->data[middle].hash == hash) {
        return &hashes->data[middle];
    } else if (hash < hashes->data[middle].hash) {
        return findHash(hashes, hash, low, middle - 1);
    } else {
        return findHash(hashes, hash, middle + 1, high);
    }
}

void freeUpdateHashes(struct UpdateHashes *hashes) {
    free(hashes->data);
    free(hashes);
}

long long update(struct CollectionData *data) {
    struct AdjacentXY adjacents[] = {
        { -1, 0 },
        { 1, 0 },
        { 0, -1 },
        { 0, 1 },
        { -1, -1 },
        { 1, -1 },
        { -1, 1 },
        { 1, 1 }
    };
    int dx;
    int dy;
    char acre;
    char **swap;
    long long iteration = 0;
    long long hash = 0;

    for (int y = 0; y < data->height; y++) {
        for (int x = 0; x < data->width; x++) {
            acre = data->area[y][x];

            data->treeAcres = 0;
            data->lumberyardAcres = 0;

            for (int a = 0; a < 8; a++) {
                dx = x + adjacents[a].x;
                dy = y + adjacents[a].y;

                if (!(dx >= 0 && dx < data->width && dy >= 0 && dy < data->height)) {
                    continue;
                }

                if (data->area[dy][dx] == '|') {
                    ++data->treeAcres;
                } else if (data->area[dy][dx] == '#') {
                    ++data->lumberyardAcres;
                }
            }

            switch (acre) {
            case '.':
                if (data->treeAcres >= 3) {
                    acre = '|';
                }
                break;
            case '|':
                if (data->lumberyardAcres >= 3) {
                    acre = '#';
                }
                break;
            case '#':
                if (!(data->lumberyardAcres >= 1 && data->treeAcres >= 1)) {
                    acre = '.';
                }
                break;
            }

            data->update[y][x] = acre;

            hash = ((hash << 5ULL) + hash) + acre;
        }
    }

    swap = data->area;
    data->area = data->update;
    data->update = swap;

    return hash;
}

int resourceValue(char * input, int iterations) {
    struct CollectionData *data = getCollectionData(input);
    int value = 0;

    if (data) {
        struct UpdateHashes *hashes = (struct UpdateHashes *)calloc(1, sizeof(struct UpdateHashes));
        long long hash;
        struct UpdateHash *updateHash;
        int interval;
        int interationsLeft;
        int intervalsLeft;

        for (int i = 0; i < iterations; i++) {
            hash = update(data);
            updateHash = findHash(hashes, hash, 0, hashes->count);

            if (updateHash) {
                interval = i - updateHash->iteration;
                interationsLeft = iterations - i - 1;
                intervalsLeft = interationsLeft / interval;

                i += intervalsLeft * interval;                
            } else {
                addHash(hashes, (struct UpdateHash){ hash, i });
            }
        }

        data->treeAcres = 0;
        data->lumberyardAcres = 0;

        for (int y = 0; y < data->height; y++) {
            for (int x = 0; x < data->width; x++) {
                if (data->area[y][x] == '|') {
                    ++data->treeAcres;
                } else if (data->area[y][x] == '#') {
                    ++data->lumberyardAcres;
                }
            }
        }

        value = data->treeAcres * data->lumberyardAcres;

        freeUpdateHashes(hashes);
        freeCollectionData(data);
    }

    return value;
}