/* Day 2, part 2 = 673 */

#include "../../aoc.h"
#include <string.h>

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        char passwordData[50];
        int positionOne;
        int positionTwo;
        char character;
        char password[20];
        char characterOne;
        char characterTwo;
        int validCount = 0;

        while (fgets(passwordData, sizeof(passwordData), inputFile)) {
            sscanf(passwordData, "%d-%d %c: %s", &positionOne, &positionTwo, &character, password);

            characterOne = password[positionOne - 1];
            characterTwo = password[positionTwo - 1];

            if (characterOne == character && characterTwo != character) {
                ++validCount;
            } else if (characterTwo == character && characterOne != character) {
                ++validCount;
            }
        }

        fclose(inputFile);

        answer = validCount;
    }

    print(aoc, INT, &answer);

    return 0;
}