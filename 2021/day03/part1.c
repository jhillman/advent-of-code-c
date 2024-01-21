/* Day 3, part 1 = 749376 */

#include "../../aoc.h"
#include <stdlib.h>
#include <string.h>
#include "bits.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        char bits[16];
        int bitsLength = 0;
        struct BitCount *bitCounts = NULL;

        while (!feof(inputFile) && fscanf(inputFile, "%s", bits)) {
            if (!bitCounts) {
                bitsLength = strlen(bits);
                bitCounts = (struct BitCount *)calloc(bitsLength, sizeof(struct BitCount));
            }

            char *bit = bits;

            while (*bit) {
                bitCounts[bit - bits].zeros += *bit == '0' ? 1 : 0;
                bitCounts[bit - bits].ones += *bit == '1' ? 1 : 0;

                ++bit;
            }
        }

        fclose(inputFile);

        int gamma = 0;
        int epsilon = 0;
        int bit = 1;

        for (int i = bitsLength - 1; i >= 0; i--) {
            gamma += bitCounts[i].ones > bitCounts[i].zeros ? bit : 0;
            epsilon += bitCounts[i].zeros > bitCounts[i].ones ? bit : 0;

            bit *= 2;
        }

        free(bitCounts);

        answer = gamma * epsilon;
    }

    print(aoc, INT, &answer);

    return 0;
}
