/* Day 5, part 2 = 661 */

#include "../../aoc.h"
#include <string.h>
#include <stdlib.h>

int compare(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        char ch;
        int seatIdCount = 0;

        while (!feof(inputFile)) {
            ch = fgetc(inputFile);

            if (ch == '\n') {
                ++seatIdCount;
            }
        }

        if (ch != '\n' && seatIdCount > 0) {
            ++seatIdCount;
        }

        char passLine[40];
        int seatIds[seatIdCount];
        int seatIdIndex = 0;

        fseek(inputFile, 0, SEEK_SET);

        while (fgets(passLine, sizeof(passLine), inputFile)) {
            int rowMin = 0;
            int rowMax = 127;
            int seatMin = 0;
            int seatMax = 7;
            
            for (int i = 0; i < 10; i++) {
                switch (passLine[i]) {
                    case 'F':
                        rowMax = rowMin + (rowMax - rowMin) / 2;
                        break;
                    case 'B':
                        rowMin = rowMax - (rowMax - rowMin) / 2;
                        break;
                    case 'L':
                        seatMax = seatMin + (seatMax - seatMin) / 2;
                        break;
                    case 'R':
                        seatMin = seatMax - (seatMax - seatMin) / 2;
                        break;
                }
            }

            seatIds[seatIdIndex++] = rowMin * 8 + seatMin;
        }

        fclose(inputFile);

        qsort(seatIds, seatIdCount, sizeof(int), compare);

        int seatMinIndex = 0;
        int seatMaxIndex = seatIdCount - 1;

        while (seatMaxIndex - seatMinIndex > 1) {
            int lowerMiddle = seatMinIndex + (seatMaxIndex - seatMinIndex) / 2;
            int upperMiddle = lowerMiddle;

            if (seatMaxIndex - lowerMiddle > lowerMiddle - seatMinIndex) {
                ++lowerMiddle;
            } else if (seatMaxIndex - upperMiddle > upperMiddle - seatMinIndex) {
                ++upperMiddle;
            }

            if (seatIds[lowerMiddle] - seatIds[seatMinIndex] > seatIds[seatMaxIndex] - seatIds[upperMiddle]) {
                seatMaxIndex = seatMinIndex + (seatMaxIndex - seatMinIndex) / 2;
            } else {
                seatMinIndex = seatMaxIndex - (seatMaxIndex - seatMinIndex) / 2;
            }
        }

        answer = seatIds[seatMinIndex] + 1;
    }

    print(aoc, INT, &answer);

    return 0;
}