/* Day 2, part 2 = 72513 */

#include "../../aoc.h"
#include <string.h>

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");

    if (inputFile) {
        char line[200];
        char *sets;
        char *cubeCount;
        int cubes;
        char color[6];
        int red;
        int green;
        int blue;
        int answer = 0;

        while (fgets(line, sizeof(line), inputFile)) {
            line[strcspn(line, "\n")] = 0;
            sets = strchr(line, ':') + 1;

            red = 0;
            green = 0;
            blue = 0;

            cubeCount = strtok(sets, ";,");

            while (cubeCount) {
                sscanf(cubeCount, "%d %s", &cubes, color);

                switch (*color) {
                case 'r':
                    if (cubes > red) {
                        red = cubes;
                    }
                    break;
                case 'g':
                    if (cubes > green) {
                        green = cubes;
                    }
                    break;
                case 'b':
                    if (cubes > blue) {
                        blue = cubes;
                    }
                    break;
                }

                cubeCount = strtok(NULL, ";,");
            }

            answer += red * green * blue;
        }

        fclose(inputFile);

        print(aoc, INT, &answer);
    }

    return 0;
}
