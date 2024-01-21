/* Day 8, part 1 = 1463 */

#include "../../aoc.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        int layerWidth = 25;
        int layerHeight = 6;

        char layerRow[layerWidth + 1];
        char layerScanFormat[7];

        sprintf(layerScanFormat, "%%%ds", layerWidth);

        int currentRow = 0;
        int zerosCount = 0;
        int onesCount = 0;
        int twosCount = 0;
        int minZeroCount = -1;
        int onesAndTwos = 0;
        char *pixelPtr;

        while (fscanf(inputFile, layerScanFormat, layerRow) > 0) {
            pixelPtr = layerRow;

            while (*pixelPtr) {
                switch (*pixelPtr) {
                    case '0':
                        ++zerosCount;
                        break;
                    case '1':
                        ++onesCount;
                        break;
                    case '2':
                        ++twosCount;
                        break;
                }

                ++pixelPtr;
            }

            if (++currentRow % layerHeight == 0) {
                if (minZeroCount < 0 || zerosCount < minZeroCount) {
                    minZeroCount = zerosCount;
                    onesAndTwos = onesCount * twosCount;
                }

                zerosCount = 0;
                onesCount = 0;
                twosCount = 0;
            }
        }

        fclose(inputFile);

        answer = onesAndTwos;
    }

    print(aoc, INT, &answer);

    return 0;
}
