/* Day 2, part 1 = 7192 */

#include "../../aoc.h"
#include <string.h>
#include <stdbool.h>

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        char line[32];
        int letters[26];
        char *letter;
        char twoLetter;
        char threeLetter;
        int twosTotal = 0;
        int threesTotal = 0;

        while (fgets(line, sizeof(line), inputFile)) {
            line[strcspn(line, "\n")] = 0;
            memset(letters, 0, 26 * sizeof(int));

            letter = line;
            twoLetter = '\0';
            threeLetter = '\0';

            while (*letter) {
                ++letters[*letter++ - 'a'];
            }

            for (int i = 0; i < 26; i++) {
                if (letters[i] == 2) {
                    twoLetter = 'a' + i;
                }

                if (letters[i] == 3) {
                    threeLetter = 'a' + i;
                }
            }

            twosTotal += (twoLetter ? 1 : 0);
            threesTotal += (threeLetter ? 1 : 0);
        }

        fclose(inputFile);

        answer = twosTotal * threesTotal;
    }

    print(aoc, INT, &answer);

    return 0;
}
