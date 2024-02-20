#include "../../aoc.h"

struct Identifier {
    int x;
    int y;
    int d;
};

struct Identifier findIdentifier(char *input, int dimension) {
    FILE *inputFile = fopen(input, "r");
    struct Identifier identifier = { 0, 0, 0 };

    if (inputFile) {
        int serialNumber;
        int powerGrid[300][300];
        int rackId;
        int powerLevel;

        fscanf(inputFile, "%d", &serialNumber);
        fclose(inputFile);

        for (int y = 1; y <= 300; y++) {
            for (int x = 1; x <= 300; x++) {
                rackId = x + 10;
                powerLevel = rackId * y;
                powerLevel += serialNumber;
                powerLevel *= rackId;
                powerLevel = (powerLevel % 1000) / 100;
                powerLevel -= 5;

                powerGrid[y - 1][x - 1] = powerLevel;
            }
        }

        int maxTotalPower = 0;
        int grid[300][300];

        for (int d = 1; d <= dimension; d++) {
            for (int y = 1; y <= 300 - d; y++) {
                for (int x = 1; x <= 300; x++) {
                    grid[y][x] += powerGrid[y + d - 2][x - 1];
                }
            }

            for (int y = 1; y <= 300 - d; y++) {
                for (int x = 1; x <= 300 - d; x++) {
                    int totalPower = 0;

                    for (int i = 0; i < d; i++) {
                        totalPower += grid[y - 1][x + i - 1];
                    }

                    if (totalPower > maxTotalPower) {
                        maxTotalPower = totalPower;
                        identifier.y = y - 1;
                        identifier.x = x - 1;
                        identifier.d = d;
                    }
                }
            }
        }
    }

    return identifier;
}