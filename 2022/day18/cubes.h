#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

void findExterior(int x, int y, int z, int minX, int maxX, int minY, int maxY, int minZ, int maxZ, bool ***grid, bool ***exterior) {
    if (((x >= minX - 1 && x <= maxX + 1 && y >= minY - 1 && y <= maxY + 1 && z >= minZ - 1 && z <= maxZ + 1) && !exterior[x + 1][y + 1][z + 1]) &&
        (!(x >= minX && x <= maxX && y >= minY && y <= maxY && z >= minZ && z <= maxZ) || !grid[x][y][z])) {
        exterior[x + 1][y + 1][z + 1] = true;

        int xDeltas[] = {-1, 1, 0, 0, 0, 0};
        int yDeltas[] = {0, 0, -1, 1, 0, 0};
        int zDeltas[] = {0, 0, 0, 0, -1, 1};

        for (int i = 0; i < 6; i++) {
            int nX = x + xDeltas[i];
            int nY = y + yDeltas[i];
            int nZ = z + zDeltas[i];

            findExterior(nX, nY, nZ, minX, maxX, minY, maxY, minZ, maxZ, grid, exterior);
        }
    }
}

int getArea(char *input, bool exteriorOnly) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        int x;
        int y;
        int z;
        int minX = 100;
        int maxX = 0;
        int minY = 100;
        int maxY = 0;
        int minZ = 100;
        int maxZ = 0;
        int area = 0;

        while (fscanf(inputFile, "%d,%d,%d", &x, &y, &z) == 3) {
            minX = min(x, minX);
            maxX = max(x, maxX);
            minY = min(y, minY);
            maxY = max(y, maxY);
            minZ = min(z, minZ);
            maxZ = max(z, maxZ);
        }

        bool ***grid = (bool ***)calloc(maxX + 1, sizeof(bool **));

        for (int x = 0; x <= maxX; x++) {
            grid[x] = (bool **)calloc(maxY + 1, sizeof(bool *));

            for (int y = 0; y <= maxY; y++) {
                grid[x][y] = (bool *)calloc(maxZ + 1, sizeof(bool));
            }
        }

        bool ***exterior;

        if (exteriorOnly) {
            exterior = (bool ***)calloc(maxX + 3, sizeof(bool **));

            for (int x = 0; x <= maxX + 2; x++) {
                exterior[x] = (bool **)calloc(maxY + 3, sizeof(bool *));

                for (int y = 0; y <= maxY + 2; y++) {
                    exterior[x][y] = (bool *)calloc(maxZ + 3, sizeof(bool));
                }
            }
        }

        fseek(inputFile, 0, SEEK_SET);

        while (fscanf(inputFile, "%d,%d,%d", &x, &y, &z) == 3) {
            grid[x][y][z] = true;
        }

        if (exteriorOnly) {
            findExterior(minX - 1, minY - 1, minZ - 1, minX, maxX, minY, maxY, minZ, maxZ, grid, exterior);
        }

        int xDeltas[] = {-1, 1, 0, 0, 0, 0};
        int yDeltas[] = {0, 0, -1, 1, 0, 0};
        int zDeltas[] = {0, 0, 0, 0, -1, 1};

        fseek(inputFile, 0, SEEK_SET);

        while (fscanf(inputFile, "%d,%d,%d", &x, &y, &z) == 3) {
            area += exteriorOnly ? 0 : 6;

            for (int i = 0; i < 6; i++) {
                int nX = x + xDeltas[i];
                int nY = y + yDeltas[i];
                int nZ = z + zDeltas[i];

                if (exteriorOnly && !(nX >= minX - 1 && nX <= maxX + 1 && nY >= minY - 1 && nY <= maxY + 1 && nZ >= minZ - 1 && nZ <= maxZ + 1)) {
                    continue;
                } else if (!exteriorOnly && !(nX >= minX && nX <= maxX && nY >= minY && nY <= maxY && nZ >= minZ && nZ <= maxZ)) {
                    continue;
                }

                if (exteriorOnly && exterior[nX + 1][nY + 1][nZ + 1]) {
                    ++area;
                } else if (!exteriorOnly && grid[nX][nY][nZ]) {
                    --area;
                }
            }
        }

        fclose(inputFile);

        for (int x = 0; x <= maxX; x++) {
            for (int y = 0; y <= maxY; y++) {
                free(grid[x][y]);
            }

            free(grid[x]);
        }

        free(grid);

        if (exteriorOnly) {
            for (int x = 0; x <= maxX + 2; x++) {
                for (int y = 0; y <= maxY + 2; y++) {
                    free(exterior[x][y]);
                }

                free(exterior[x]);
            }

            free(exterior);
        }

        return area;
    }

    return 0;
}