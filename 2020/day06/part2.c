/* Day 6, part 2 = 3103 */

#include "../../aoc.h"
#include <string.h>

void resetCharacterCounts(int characterCounts[]) {
    for (int i = 0; i < 26; i++) {
        characterCounts[i] = 0;
    }
}

int answerCount(int characterCounts[], int groupSize) {
    int count = 0;

    for (int i = 0; i < 26; i++) {
        if (characterCounts[i] == groupSize) {
            count += 1;
        }
    }

    return count;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        char answersLine[50];
        char answersLineLength;
        int characterCounts[26];
        int groupSize = 0;
        int sum = 0;

        resetCharacterCounts(characterCounts);

        while (fgets(answersLine, sizeof(answersLine), inputFile)) {
            answersLine[strcspn(answersLine, "\n")] = 0;
            answersLineLength = strlen(answersLine);

            if (answersLineLength == 0) {
                sum += answerCount(characterCounts, groupSize);

                resetCharacterCounts(characterCounts);

                groupSize = 0;
            } else {
                for (int i = 0; i < answersLineLength; i++) {
                    ++characterCounts[answersLine[i] - 'a'];
                }

                ++groupSize;
            }
        }

        fclose(inputFile);

        sum += answerCount(characterCounts, groupSize);

        answer = sum;
    }

    print(aoc, INT, &answer);

    return 0;
}