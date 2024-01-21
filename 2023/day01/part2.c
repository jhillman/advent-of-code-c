/* Day 1, part 2 = 54094 */

#include "../../aoc.h"
#include <ctype.h>
#include <string.h>

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");

    if (inputFile) {
        char line[64];
        char *character;
        int digit1;
        int digit2;
        char *digits[] = {
            "one",
            "two",
            "three",
            "four",
            "five",
            "six",
            "seven",
            "eight",
            "nine"
        };
        char *digit1Ptr;
        char *digit2Ptr;
        int answer = 0;

        while (fgets(line, sizeof(line), inputFile)) {
            line[strcspn(line, "\n")] = 0;

            character = line;
            digit1 = -1;
            digit2 = -1;

            while (*character) {
                if (isdigit(*character)) {
                    if (digit1 == -1) {
                        digit1 = (*character - '0');
                        digit1Ptr = character;
                    }

                    digit2 = (*character - '0');
                    digit2Ptr = character;
                }

                character++;
            }

            for (int i = 0; i < 9; i++) {
                char *digitPtr = strstr(line, digits[i]);

                while (digitPtr) {
                    if (digitPtr && (digit1 == -1 || digitPtr < digit1Ptr)) {
                        digit1Ptr = digitPtr;
                        digit1 = i + 1;
                    }

                    if (digitPtr && (digit2 == -1 || digitPtr > digit2Ptr)) {
                        digit2Ptr = digitPtr;
                        digit2 = i + 1;
                    }

                    digitPtr = strstr(digitPtr + 1, digits[i]);
                }   
            }

            answer += digit1 * 10 + digit2;
        }

        fclose(inputFile);

        print(aoc, INT, &answer);
    }

    return 0;
}
