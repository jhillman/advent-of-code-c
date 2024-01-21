/* Day 25, part 1 = 429 */

#include "../../aoc.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

void printSeaCucumbers(char **seaCucumbers, int width, int height) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            printf("%c", seaCucumbers[y][x]);
        }

        printf("\n");
    }    

    printf("\n");
}

bool move(char **seaCucumbers, int width, int height) {
    bool moved = false;

    // east
    for (int y = 0; y < height; y++) {
        bool canWrap = true;

        for (int x = 0; x < width; x++) {
            if (x == 0 && seaCucumbers[y][x] == '>') {
                canWrap = false;
            }

            if (seaCucumbers[y][x] == '>') {
                int xDestination = x == width - 1 ? 0 : x + 1;

                if (seaCucumbers[y][xDestination] == '.' && (xDestination > 0 || canWrap)) {
                    seaCucumbers[y][x] = '.';
                    seaCucumbers[y][xDestination] = '>';

                    if (x < width - 1) {
                        ++x;
                    }

                    moved = true;
                }
            }
        }
    }    

    // south
    for (int x = 0; x < width; x++) {
        bool canWrap = true;

        for (int y = 0; y < height; y++) {
            if (y == 0 && seaCucumbers[y][x] == 'v') {
                canWrap = false;
            }

            if (seaCucumbers[y][x] == 'v') {
                int yDestination = y == height - 1 ? 0 : y + 1;

                if (seaCucumbers[yDestination][x] == '.' && (yDestination > 0 || canWrap)) {
                    seaCucumbers[y][x] = '.';
                    seaCucumbers[yDestination][x] = 'v';

                    if (y < height - 1) {
                        ++y;
                    }

                    moved = true;
                }
            }
        }
    }    

    return moved;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        char ch;
        int width = 0;
        int height = 1;
        char **seaCucumbers;

        while (fscanf(inputFile, "%c", &ch) >= 1) {
            if (ch == '\n') {
                ++height;
            } else if (height == 1) {
                ++width;
            }
        }

        fseek(inputFile, 0, SEEK_SET);

        seaCucumbers = (char **)malloc(height * sizeof(char *));

        for (int y = 0; y < height; y++) {
            seaCucumbers[y] = (char *)malloc(width * sizeof(char));

            for (int x = 0; x < width; x++) {
                fscanf(inputFile, "%c", &seaCucumbers[y][x]);
            }

            fscanf(inputFile, "%c", &ch); // \n
        }

        fclose(inputFile);

        int step = 0;

        while (++step && move(seaCucumbers, width, height));

        for (int y = 0; y < height; y++) {
            free(seaCucumbers[y]);
        }

        free(seaCucumbers);

        answer = step;
    }

    print(aoc, INT, &answer);

    return 0;
}


