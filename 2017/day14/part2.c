/* Day 14, part 2 = 1018 */

#include "../../aoc.h"
#include "../knot.h"
#include <stdbool.h>

void clearRegion(char grid[128][128], int y, int x) {
    if (grid[y][x] != '1') {
        return;
    }

    grid[y][x] = '0';

    int xDeltas[] = {0, -1, 1, 0};
    int yDeltas[] = {-1, 0, 0, 1};
    int dX;
    int dY;    

    for (int i = 0; i < 4; i++) {
        dX = x + xDeltas[i];
        dY = y + yDeltas[i];

        if (dX >= 0 && dX < 128 && dY >= 0 && dY < 128) {
            clearRegion(grid, dY, dX);
        }
    }
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        char key[16];
        char keyRow[16];
        char hashedKeyRow[33];
        char grid[128][128];
        char *g;
        char *h;
        int value;
        char *bits[16] = {
            "0000", "0001", "0010", "0011",
            "0100", "0101", "0110", "0111",
            "1000", "1001", "1010", "1011",
            "1100", "1101", "1110", "1111"
        };
        int regions = 0;

        fgets(key, sizeof(key), inputFile);
        fclose(inputFile);

        for (int i = 0; i < 128; i++) {
            sprintf(keyRow, "%s-%d", key, i);
            hash(keyRow, hashedKeyRow);

            g = grid[i];
            h = hashedKeyRow;

            for (int j = 0; j < 32; j++) {
                sscanf(h++, "%1x", &value);
                sprintf(g, "%s", bits[value]);

                g += 4;
            }
        }

        for (int y = 0; y < 128; y++) {
            for (int x = 0; x < 128; x++) {
                if (grid[y][x] == '1') {
                    ++regions;
                    clearRegion(grid, y, x);
                }
            }
        }

        answer = regions;
    }

    print(aoc, INT, &answer);

    return 0;
}