/* Day 4, part 2 = 119 */

#include "../../aoc.h"
#include <string.h>
#include <stdbool.h>

bool anagrams(char *first, char *second) {
    if (strlen(first) != strlen(second)) {
        return false;
    }

    int wordLength = strlen(first);
    int firstLetterCounts[26] = { 0 };
    int secondLetterCounts[26] = { 0 };

    for (int i = 0; i < wordLength; i++) {
        ++firstLetterCounts[first[i] - 'a'];
        ++secondLetterCounts[second[i] - 'a'];
    }

    bool sameCounts = true;

    for (int i = 0; sameCounts && i < 26; i++) {
        sameCounts = firstLetterCounts[i] == secondLetterCounts[i];
    }

    return sameCounts;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        char phrase[128];
        char words[128];
        char others[128];
        char *word;
        char *wordContext;
        char *other;
        char *otherContext;
        bool valid;
        int totalValid = 0;

        while (fgets(phrase, sizeof(phrase), inputFile)) {
            phrase[strcspn(phrase, "\n")] = 0;
            strcpy(words, phrase);

            word = strtok_r(words, " ", &wordContext);
            valid = true;
   
            while (valid && word) {
                strcpy(others, phrase + (word - words) + strlen(word) + 1);
                other = strtok_r(others, " ", &otherContext);

                while (valid && other) {
                    valid = !anagrams(word, other);

                    other = strtok_r(NULL, " ", &otherContext);
                }

                word = strtok_r(NULL, " ", &wordContext);
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