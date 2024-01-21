/* Day 18, part 1 = 14006719520523 */

#include "../../aoc.h"
#include "math.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    long answer = 0;

    if (inputFile) {
        char expression[200];
        long sum = 0;

        while (fgets(expression, sizeof(expression), inputFile)) {
            sum += evaluate(expression, NULL, false);
        }

        fclose(inputFile);

        answer = sum;
    }

    print(aoc, LONG, &answer);

    return 0;
}
