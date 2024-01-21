/* Day 3, part 2 = 2372923 */

#include "../../aoc.h"
#include <stdlib.h>
#include <string.h>
#include "bits.h"

#pragma clang optimize off

char bit = '0';
int bitPosition = 0;

int compare(const void *a, const void *b) {
    char *aBits = *(char **)(bit == '1' ? b : a);
    char *bBits = *(char **)(bit == '1' ? a : b);

    return *(aBits + bitPosition) - *(bBits + bitPosition);
}

char getCurrentBit(struct BitCount bitCount, enum BitPriority priority) {
    return bitCount.ones >= bitCount.zeros ? (priority == MOST_COMMON ? '1' : '0') : (priority == MOST_COMMON ? '0' : '1');
}

int getRating(struct BitCount bitCount, char **ratings, int ratingCount, enum BitPriority priority) {
    bitPosition = 0;
    int remainingRatingCount = ratingCount;

    do {
        bit = getCurrentBit(bitCount, priority);

        qsort(ratings, remainingRatingCount, sizeof(char **), compare);

        remainingRatingCount = 0;
        bitCount.ones = 0;
        bitCount.zeros = 0;
        ++bitPosition;

        for (int i = 0; i < ratingCount; i++) {
            if (*(ratings[i] + bitPosition - 1) == bit) {
                bitCount.zeros += *(ratings[i] + bitPosition) == '0' ? 1 : 0;
                bitCount.ones += *(ratings[i] + bitPosition) == '1' ? 1 : 0;

                ++remainingRatingCount;
            } else {
                break;
            }
        }
    } while (remainingRatingCount > 1);

    int rating = 0;
    int bitsLength = strlen(*ratings);
    int currentBit = 1;

    for (int i = bitsLength - 1; i >= 0; i--) {
        rating += (*ratings)[i] == '1' ? currentBit : 0;

        currentBit *= 2;
    }

    return rating;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        char bits[16];
        int bitsLength = 0;
        struct BitCount bitCount = {0, 0};
        int ratingCount = 0;

        while (!feof(inputFile) && fscanf(inputFile, "%s", bits)) {
            bitCount.zeros += *bits == '0' ? 1 : 0;
            bitCount.ones += *bits == '1' ? 1 : 0;

            ++ratingCount;
        }

        fseek(inputFile, 0, SEEK_SET);

        char **oxygenGeneratorRatings = (char **)calloc(ratingCount, sizeof(char *));
        char **co2ScrubberRatings = (char **)calloc(ratingCount, sizeof(char *));
        int ratingPosition = 0;

        while (!feof(inputFile) && fscanf(inputFile, "%s", bits)) {
            oxygenGeneratorRatings[ratingPosition] = (char *)calloc(bitsLength, sizeof(char));
            co2ScrubberRatings[ratingPosition] = (char *)calloc(bitsLength, sizeof(char));

            strcpy(oxygenGeneratorRatings[ratingPosition], bits);
            strcpy(co2ScrubberRatings[ratingPosition], bits);

            ++ratingPosition;
        }

        fclose(inputFile);

        int oxygenGeneratorRating = getRating(bitCount, oxygenGeneratorRatings, ratingCount, MOST_COMMON);
        int co2ScrubberRating = getRating(bitCount, co2ScrubberRatings, ratingCount, LEAST_COMMON);

        for (int i = 0; i < ratingCount; i++) {
            free(oxygenGeneratorRatings[i]);
            free(co2ScrubberRatings[i]);
        }

        free(oxygenGeneratorRatings);
        free(co2ScrubberRatings);

        answer = oxygenGeneratorRating * co2ScrubberRating;
    }

    print(aoc, INT, &answer);

    return 0;
}

#pragma clang optimize on
