/* Day 12, part 2 = 518311327635164 */

#include "../../aoc.h"
#include "moons.h"
#include <string.h>
#include <stdbool.h>

char *axisState(struct Moon *moons, int moonCount, enum Axis axis) {
    char *state = (char *)calloc(64, sizeof(char));
    char axisPair[32];

    for (int i = 0; i < moonCount; i++) {
        sprintf(axisPair, "%d,%d", moons[i].position[axis], moons[i].velocity[axis]);

        strcat(state, axisPair);
        
        if (i < moonCount - 1) {
            strcat(state, "|");
        }
    }

    return state;
}

long gcd(long a, long b) {
    long temp;

    while (b != 0) {
        temp = b;
        b = a % b;
        a = temp;
    }

    return a;
}

long lcm(long a, long b) {
    return a * b / gcd(a, b);
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    int moonCount = 0;
    struct Moon *moons = getMoons(aoc.input, &moonCount);
    long answer = 0;

    if (moons) {
        char *xStateStart = axisState(moons, moonCount, X);
        char *yStateStart = axisState(moons, moonCount, Y);
        char *zStateStart = axisState(moons, moonCount, Z);

        int steps = 0;
        int xSteps = 0;
        int ySteps = 0;
        int zSteps = 0;

        while (xSteps == 0 || ySteps == 0 || zSteps == 0) {
            ++steps;
            stepMoons(moons, moonCount);

            if (xSteps == 0) {
                char *xState = axisState(moons, moonCount, X);

                if (strcmp(xState, xStateStart) == 0) {
                    xSteps = steps;
                }

                free(xState);
            }

            if (ySteps == 0) {
                char *yState = axisState(moons, moonCount, Y);

                if (strcmp(yState, yStateStart) == 0) {
                    ySteps = steps;
                }

                free(yState);
            }

            if (zSteps == 0) {
                char *zState = axisState(moons, moonCount, Z);

                if (strcmp(zState, zStateStart) == 0) {
                    zSteps = steps;
                }

                free(zState);
            }
        }

        free(xStateStart);
        free(yStateStart);
        free(zStateStart);

        answer = lcm(xSteps, lcm(ySteps, zSteps));
    }

    print(aoc, LONG, &answer);

    return 0;
}
