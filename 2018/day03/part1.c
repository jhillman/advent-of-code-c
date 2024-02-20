/* Day 3, part 1 = 116140 */

#include "../../aoc.h"
#include <stdlib.h>

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        int **fabric;
        int id;
        int edgeX;
        int edgeY;
        int width;
        int height;

        fabric = (int **)malloc(1000 * sizeof(int *));

        for (int y = 0; y < 1000; y++) {
            fabric[y] = (int *)calloc(1000, sizeof(int));
        }

        while (fscanf(inputFile, "#%d @ %d,%d: %dx%d\n", &id, &edgeX, &edgeY, &width, &height) == 5) {
            for (int y = edgeY; y < edgeY + height; y++) {
                for (int x = edgeX; x < edgeX + width; x++) {
                    ++fabric[y][x];
                }
            }
        }

        for (int y = 0; y < 1000; y++) {
            for (int x = 0; x < 1000; x++) {
                answer += (fabric[y][x] > 1 ? 1 : 0);
            }

            free(fabric[y]);
        }

        free(fabric);

        fclose(inputFile);
    }

    print(aoc, INT, &answer);

    return 0;
}
