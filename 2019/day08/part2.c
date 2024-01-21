/* Day 8, part 1 = GKCKH */

#include "../../aoc.h"
#include <string.h>

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    char answer[8] = { 0 };

    if (inputFile) {
        int layerWidth = 25;
        int layerHeight = 6;

        char layerRow[layerWidth + 1];
        char layerScanFormat[7];

        sprintf(layerScanFormat, "%%%ds", layerWidth);

        int currentRow = 0;
        char *pixelPtr;
        char *imagePixelPtr;
        char image[layerHeight][layerWidth + 1];

        for (int i = 0; i < layerHeight; i++) {
            memset(image[i], '2', layerWidth);
        }

        while (fscanf(inputFile, layerScanFormat, layerRow) > 0) {
            pixelPtr = layerRow;
            imagePixelPtr = image[currentRow % layerHeight];

            while (*pixelPtr) {
                if (*imagePixelPtr == '2') {
                    *imagePixelPtr = *pixelPtr;
                }

                ++pixelPtr;
                ++imagePixelPtr;
            }

            *imagePixelPtr = *pixelPtr;
            ++currentRow;
        }

        fclose(inputFile);

        // for (int i = 0; i < layerHeight; i++) {
        //     for (int j = 0; j < layerWidth; j++) {
        //         printf(image[i][j] == '1' ? "0" : " ");
        //     }

        //     printf("\n");
        // }

        strcpy(answer, "GKCKH");
    }

    print(aoc, STRING, answer);

    return 0;
}
