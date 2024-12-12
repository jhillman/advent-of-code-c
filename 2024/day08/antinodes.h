#include <stdlib.h>
#include <stdbool.h>

struct Location {
    int x;
    int y;
};

struct Locations {
    int capacity;
    int count;
    struct Location *data;
};

void addLocation(struct Locations *locations, struct Location location) {
    if (locations->count == locations->capacity) {
        locations->capacity += 5;
        locations->data = (struct Location *)realloc(locations->data, locations->capacity * sizeof(struct Location));
    }

    locations->data[locations->count++] = location;
}

void freeLocations(struct Locations *locations) {
    free(locations->data);
    free(locations);
}

struct Antenna {
    char id;
    struct Locations *locations;
};

struct Antennae {
    int capacity;
    int count;
    struct Antenna **data;
};

int compare(const void *a, const void *b) {
    struct Antenna *antennaA = *(struct Antenna **)a;
    struct Antenna *antennaB = *(struct Antenna **)b;

    return antennaA->id - antennaB->id;
}

void addAntenna(struct Antennae *antennae, struct Antenna *antenna) {
    if (antennae->count == antennae->capacity) {
        antennae->capacity += 5;
        antennae->data = (struct Antenna **)realloc(antennae->data, antennae->capacity * sizeof(struct Antenna *));
    }

    antennae->data[antennae->count++] = antenna;

    qsort(antennae->data, antennae->count, sizeof(struct Antenna *), compare);
}

void freeAntennae(struct Antennae *antennae) {
    for (int i = 0; i < antennae->count; i++) {
        free(antennae->data[i]->locations->data);
        free(antennae->data[i]->locations);
    }

    free(antennae->data);
    free(antennae);
}

struct Antenna *findAntenna(struct Antennae *antennae, char id, int low, int high) {
    if (antennae->count == 0 || low > high) {
        return NULL;
    }

    int middle = low + ((high - low) / 2);

    if (middle < antennae->count) {
        int result = id - antennae->data[middle]->id;

        if (result == 0) {
            return antennae->data[middle];
        } else if (result < 0) {
            return findAntenna(antennae, id, low, middle - 1);
        } else {
            return findAntenna(antennae, id, middle + 1, high);
        }
    }

    return NULL;
}

struct Antenna *getAntenna(struct Antennae *antennae, char id) {
    struct Antenna *antenna = findAntenna(antennae, id, 0, antennae->count);

    if (antenna == NULL) {
        antenna = (struct Antenna *)calloc(1, sizeof(struct Antenna));
        antenna->id = id;
        antenna->locations = (struct Locations *)calloc(1, sizeof(struct Locations));

        addAntenna(antennae, antenna);
    }


    return antenna;
}

struct Map {
    int width;
    int height;
    struct Antennae *antennae;
    bool **antinodes;
    int antinodeCount;
};

void addAntinode(struct Map *map, struct Location location, struct Location delta, int mulitplier, bool resonant) {
    struct Location antinode = (struct Location){ location.x + (delta.x * mulitplier), location.y + (delta.y * mulitplier) };

    if (resonant && !map->antinodes[location.y][location.x]) {
        map->antinodes[location.y][location.x] = true;
        ++map->antinodeCount;
    }

    if (!(antinode.x < 0 || antinode.x >= map->width || antinode.y < 0 || antinode.y >= map->height)) {
        if (!map->antinodes[antinode.y][antinode.x]) {
            map->antinodes[antinode.y][antinode.x] = true;
            ++map->antinodeCount;
        }

        if (resonant) {
            addAntinode(map, antinode, delta, mulitplier, resonant);
        }
    }
}

void freeMap(struct Map *map) {
    freeAntennae(map->antennae);

    for (int y = 0; y < map->height; y++) {
        free(map->antinodes[y]);
    }

    free(map->antinodes);
    free(map);
}

int getAntinodeCount(char *input, bool resonant) {
    FILE *inputFile = fopen(input, "r");
    int count = 0;

    if (inputFile) {
        char c;
        
        struct Map *map = (struct Map *)calloc(1, sizeof(struct Map));
        map->antennae = (struct Antennae *)calloc(1, sizeof(struct Antennae));

        while (!feof(inputFile)) {
            c = fgetc(inputFile);

            if (c == '\n' || feof(inputFile)) {
                ++map->height;
            }

            if (!map->height) {
                ++map->width;
            }
        }

        fseek(inputFile, 0, SEEK_SET);

        map->antinodes = (bool **)malloc(map->height * sizeof(bool *));

        for (int y = 0; y < map->height; y++) {
            map->antinodes[y] = (bool *)calloc(map->width, sizeof(bool));

            for (int x = 0; x < map->width; x++) {
                c = fgetc(inputFile);

                if (c != '.') {
                    struct Antenna *antenna = getAntenna(map->antennae, c);

                    addLocation(antenna->locations, (struct Location){ x, y });
                }
            }

            fgetc(inputFile); // \n
        }

        fclose(inputFile);

        for (int i = 0; i < map->antennae->count; i++) {
            for (int j = 0; j < map->antennae->data[i]->locations->count; j++) {
                for (int k = 0; k < map->antennae->data[i]->locations->count; k++) {
                    if (k != j) {
                        struct Location first = map->antennae->data[i]->locations->data[j];
                        struct Location second = map->antennae->data[i]->locations->data[k];

                        struct Location delta = { second.x - first.x, second.y - first.y };

                        addAntinode(map, first, delta, -1, resonant);
                        addAntinode(map, second, delta, 1, resonant);
                    }
                }
            }
        }

        count = map->antinodeCount;

        freeMap(map);
    }

    return count;
}
