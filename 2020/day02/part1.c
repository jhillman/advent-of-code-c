/* Day 2, part 1 = 655 */

#include "../../aoc.h"
#include <string.h>

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        char passwordData[50];
        int rangeStart;
        int rangeEnd;
        char character;
        char password[20];
        int passwordLength;
        int characterCount;
        int validCount = 0;

        while (fgets(passwordData, sizeof(passwordData), inputFile)) {
            sscanf(passwordData, "%d-%d %c: %s", &rangeStart, &rangeEnd, &character, password);

            passwordLength = strlen(password);
            characterCount = 0;

            for (int i = 0; i < passwordLength; i++) {
                if (*(password + i) == character) {
                    ++characterCount;
                }
            }

            if (characterCount >= rangeStart && characterCount <= rangeEnd) {
                ++validCount;
            }
        }

        fclose(inputFile);

        answer = validCount;
    }

    print(aoc, INT, &answer);

    return 0;
}