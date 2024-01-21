#include <stdlib.h>
#include <string.h>

struct Orbit {
    char inner[4];
    char outer[4];
};

struct Object {
    char name[4];
    struct Object *orbitee;
    struct Object *orbiters;
    int orbiterCount;
};

int orbiterCount(struct Orbit *orbits, int orbitCount, char *name) {
    int count = 0;

    for (int i = 0; i < orbitCount; i++) {
        if (strcmp(orbits[i].inner, name) == 0) {
            ++count;
        }
    }

    return count;
}

struct Orbit findOrbit(struct Orbit *orbits, int orbitCount, char *name, int *index) {
    int orbitIndex = -1;

    for (int i = *index; orbitIndex < 0 && i < orbitCount; i++) {
        if (strcmp(orbits[i].inner, name) == 0) {
            orbitIndex = i;
        }
    }

    *index = orbitIndex + 1;

    return orbits[orbitIndex];
}

struct Object *findObject(struct Object *object, char *name) {
    struct Object *targetObject = NULL;

    if (strcmp(object->name, name) == 0) {
        targetObject = object;
    } else {
        for (int i = 0; !targetObject && i < object->orbiterCount; i++) {
            targetObject = findObject(&object->orbiters[i], name);
        }
    }

    return targetObject;
}

void buildOrbitMap(struct Object *object, struct Object *orbitee, struct Orbit *orbits, int orbitCount) {
    object->orbitee = orbitee;
    object->orbiterCount = orbiterCount(orbits, orbitCount, object->name);;
    object->orbiters = (struct Object *) calloc(object->orbiterCount, sizeof(struct Object));

    int index = 0;
    int orbiterIndex = 0;

    while (orbiterIndex < object->orbiterCount) {
        struct Orbit orbit = findOrbit(orbits, orbitCount, object->name, &index);

        strcpy(object->orbiters[orbiterIndex].name, orbit.outer);

        buildOrbitMap(&object->orbiters[orbiterIndex], object, orbits, orbitCount);

        ++orbiterIndex;
    }
}

void freeObject(struct Object *object) {
    for (int i = 0; i < object->orbiterCount; i++) {
        freeObject(&object->orbiters[i]);
    }

    free(object->orbiters);
}

void freeMap(struct Object *orbitMap) {
    freeObject(orbitMap);
    free(orbitMap);
}

struct Object *readInput(char *input) {
    FILE *inputFile = fopen(input, "r");

    struct Object *orbitMap = NULL;

    if (inputFile) {
        char ch;
        int orbitCount = 0;

        while (!feof(inputFile)) {
            ch = fgetc(inputFile);

            if (ch == '\n') {
                ++orbitCount;
            }
        }

        if (ch != '\n' && orbitCount > 0) {
            ++orbitCount;
        }

        fseek(inputFile, 0, SEEK_SET);

        struct Orbit *orbits = (struct Orbit *) calloc(orbitCount, sizeof(struct Orbit));

        int orbitIndex = 0;

        while (orbitIndex < orbitCount) {
            fscanf(inputFile, "%3s%c%3s", orbits[orbitIndex].inner, &ch, orbits[orbitIndex].outer);

            ++orbitIndex;
        }

        fclose(inputFile);

        orbitMap = (struct Object *) calloc(1, sizeof(struct Object));

        strcpy(orbitMap->name, "COM");

        buildOrbitMap(orbitMap, NULL, orbits, orbitCount);

        free(orbits);
    }

    return orbitMap;
}
