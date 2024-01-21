/* Day 2, part 1 = 2162 */

#include "../../aoc.h"
#include <string.h>
#include <stdbool.h>

#define MAX_RED 12
#define MAX_GREEN 13
#define MAX_BLUE 14

bool subsetPossible(char *subset) {
    int red;
    int green;
    int blue;
    char *context;
    char *cubeCount = strtok_r(subset, ",", &context);
    int cubes;
    char color[6];
    bool possible = true;

    while (possible && cubeCount) {
        sscanf(cubeCount, "%d %s", &cubes, color);

        switch (*color) {
        case 'r':
            possible = cubes <= MAX_RED;
            break;
        case 'g':
            possible = cubes <= MAX_GREEN;
            break;
        case 'b':
            possible = cubes <= MAX_BLUE;
            break;
        }

        cubeCount = strtok_r(NULL, ",", &context);
    }

    return possible;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");

    if (inputFile) {
        char line[200];
        int game;
        char *sets;
        char *subset;
        char *context;
        bool subsetsPossible;
        int answer = 0;

        while (fgets(line, sizeof(line), inputFile)) {
            line[strcspn(line, "\n")] = 0;

            sscanf(line, "Game %d", &game);
            sets = strchr(line, ':') + 1;

            subsetsPossible = true;
            subset = strtok_r(sets, ";", &context);

            while (subsetsPossible && subset) {
                subsetsPossible = subsetPossible(subset);

                subset = strtok_r(NULL, ";", &context);
            }

            if (subsetsPossible) {
                answer += game;
            }
        }

        fclose(inputFile);

        print(aoc, INT, &answer);
    }

    return 0;
}
