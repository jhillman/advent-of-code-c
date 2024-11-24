/* Day 2, part 1 = 45351 */

#include "../../aoc.h"

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        int value;
        char c;
        int minimum = 0;
        int maximum = 0;
        int sum = 0;

        while (!feof(inputFile) && fscanf(inputFile, "%d%c", &value, &c)) {
            minimum = !minimum ? value : min(minimum, value);
            maximum = !maximum ? value : max(maximum, value);

            if (feof(inputFile) || c == '\n') {
                sum += maximum - minimum;

                minimum = 0;
                maximum = 0;
            }
        }

        fclose(inputFile);

        answer = sum;
    }

    print(aoc, INT, &answer);

    return 0;
}
