#include <stdio.h>
#include <stdlib.h>

enum Axis {
    X,
    Y,
    Z
};

struct Moon {
    int position[3];
    int velocity[3];
};

void applyGravity(struct Moon *one, struct Moon *two) {
    for (int axis = 0; axis < 3; axis++) {
        if (one->position[axis] > two->position[axis]) {
            --one->velocity[axis];
            ++two->velocity[axis];
        } else if (one->position[axis] < two->position[axis]) {
            ++one->velocity[axis];
            --two->velocity[axis];
        }
    }
}

void applyVelocity(struct Moon *moon) {
    for (int axis = 0; axis < 3; axis++) {
        moon->position[axis] += moon->velocity[axis];
    }
}

void stepMoons(struct Moon *moons, int moonCount) {
    for (int i = 0; i < moonCount; i++) {
        for (int j = i + 1; j < moonCount; j++) {
            applyGravity(&moons[i], &moons[j]);
        }

        applyVelocity(&moons[i]);
    }
}

void printMoons(struct Moon *moons, int moonCount) {
    for (int i = 0; i < moonCount; i++) {
        printf("pos=<x=%d, y=%d, z=%d>, vel=<x=%d, y=%d, z=%d>\n", 
            moons[i].position[X], moons[i].position[Y], moons[i].position[Z], 
            moons[i].velocity[X], moons[i].velocity[Y], moons[i].velocity[Z]);
    }
}

int moonEnergy(struct Moon moon) {
    int potential = 0;
    int kinetic = 0;

    for (int axis = 0; axis < 3; axis++) {
        potential += abs(moon.position[axis]);
        kinetic += abs(moon.velocity[axis]);
    }

    return potential * kinetic;
}

struct Moon *getMoons(char *input, int *count) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        char ch;
        int moonCount = 0;

        while (fscanf(inputFile, "%c", &ch) >= 1) {
            if (ch == '<') {
                ++moonCount;
            }
        }

        fseek(inputFile, 0, SEEK_SET);

        struct Moon *moons = (struct Moon *)calloc(moonCount, sizeof(struct Moon));

        for (int i = 0; i < moonCount; i++) {
            fscanf(inputFile, "<x=%d, y=%d, z=%d>%c", 
                &moons[i].position[X], &moons[i].position[Y], &moons[i].position[Z], &ch);
        }

        fclose(inputFile);

        *count = moonCount;

        return moons;
    }

    return NULL;
}