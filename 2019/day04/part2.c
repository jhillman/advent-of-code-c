/* Day 4, part 2 = 1145 */

#include "../../aoc.h"
#include <stdbool.h>

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        int rangeMin;
        char dash;
        int rangeMax;

        fscanf(inputFile, "%d%c%d", &rangeMin, &dash, &rangeMax);

        fclose(inputFile);

        char password[7];        
        int passwordCount = 0;

        for (int passwordNumber = rangeMin; passwordNumber <= rangeMax; passwordNumber++) {
            sprintf(password, "%d", passwordNumber);

            bool increasing = true;
            bool twoAdjacent = false;

            for (int i = 1; increasing && i < 6; i++) {
                int diff = password[i] - password[i - 1];
                increasing = diff >= 0;

                if (diff == 0 && !twoAdjacent) {
                    twoAdjacent = true;

                    if (i > 1 && password[i - 2] == password[i]) {
                        twoAdjacent = false;
                    } else if (i < 5 && password[i + 1] == password[i]) {
                        twoAdjacent = false;
                    }
                }
            }

            if (increasing && twoAdjacent) {
                ++passwordCount;
            }
        }

        answer = passwordCount;
    }

    print(aoc, INT, &answer);

    return 0;
}
