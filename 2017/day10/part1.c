/* Day 10, part 1 = 1935 */

#include "../../aoc.h"
#include "hash.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        struct Numbers *numbers = getNumbers();
        int length;
        char c;

        while (!feof(inputFile) && fscanf(inputFile, "%d%c", &length, &c)) {
            knot(numbers, length);
        }

        fclose(inputFile);

        answer = numbers->list[0] * numbers->list[1];

        free(numbers);
    }

    print(aoc, INT, &answer);

    return 0;
}
