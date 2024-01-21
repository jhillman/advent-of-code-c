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

    if (!inputFile) {
        inputFile = fopen("../input.txt", "r");
    }

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
        int lowestSeatId = 1024;
        int highestSeatId = 0;

        fseek(inputFile, 0, SEEK_SET);

        while (fgets(passLine, sizeof(passLine), inputFile)) {
            int rowBit = 64;
            int seatBit = 4;
            int row = 0;
            int seat = 0;

            for (int i = 0; i < 7; i++) {
                if (passLine[i] == 'B') {
                    row += rowBit;
                }

                rowBit >>= 1;
            }

            for (int i = 7; i < 10; i++) {
                if (passLine[i] == 'R') {
                    seat += seatBit;
                }

                seatBit >>= 1;
            }

            int seatId = row * 8 + seat;

            if (seatId > highestSeatId) {
                highestSeatId = seatId;
            }

            if (seatId < lowestSeatId) {
                lowestSeatId = seatId;
            }

            seatIds[seatIdIndex++] = seatId;
        }

        fclose(inputFile);

        int seatId = 0;

        for (int i = 0; i < lowestSeatId; i++) {
            seatId ^= i;
        }

        for (int i = highestSeatId + 1; i < 1024; i++) {
            seatId ^= i;
        }

        for (int i = 0; i < seatIdCount; i++) {
            seatId ^= seatIds[i];
        }

        printf("%d", seatId);
    }

    print(aoc, INT, &answer);

    return 0;
}