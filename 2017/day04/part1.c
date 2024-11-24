/* Day 4, part 1 = 325 */

#include "../../aoc.h"
#include <string.h>
#include <stdbool.h>

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        char phrase[128];
        char phraseCopy[128];
        char *word;
        char *duplicate;
        char before;
        char after;
        bool valid;
        int totalValid = 0;

        while (fgets(phrase, sizeof(phrase), inputFile)) {
            phrase[strcspn(phrase, "\n")] = 0;
            strcpy(phraseCopy, phrase);

            word = strtok(phrase, " ");
            valid = true;
   
            while (word) {
                duplicate = strstr(phraseCopy + (word - phrase) + strlen(word), word);

                if (duplicate) {
                    before = *(duplicate - 1);
                    after = *(duplicate + strlen(word));

                    if (before == ' ' && (after == ' ' || after == 0)) {
                        valid = false;
                        break;
                    }
                }

                word = strtok(NULL, " ");
            }

            if (valid) {
                ++totalValid;
            }
        }

        fclose(inputFile);

        answer = totalValid;
    }

    print(aoc, INT, &answer);

    return 0;
}
