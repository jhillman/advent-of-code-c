/* Day 2, part 2 = mbruvapghxlzycbhmfqjonsie */

#include "../../aoc.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    char answer[32];

    if (inputFile) {
        char line[32];
        int boxCount = 0;
        char **boxIds;
        int idLength;
        bool answerFound = false;

        while (fgets(line, sizeof(line), inputFile)) {
            ++boxCount;
        }

        fseek(inputFile, 0, SEEK_SET);

        boxIds = (char **)malloc(boxCount * sizeof(char *));

        for (int i = 0; i < boxCount; i++) {
            boxIds[i] = malloc(32 * sizeof(char));
            fgets(boxIds[i], 32 * sizeof(char), inputFile);

            boxIds[i][strcspn(boxIds[i], "\n")] = 0;
        }

        fclose(inputFile);

        idLength = strlen(boxIds[0]);

        for (int i = 0; !answerFound && i < boxCount; i++) {
            for (int j = i + 1; !answerFound && j < boxCount; j++) {
                for (int k = 0; !answerFound && k < idLength; k++) {
                    if (strncmp(boxIds[i], boxIds[j], k) == 0 && strcmp(boxIds[i] + k + 1, boxIds[j] + k + 1) == 0) {
                        boxIds[i][k] = '\0';

                        sprintf(answer, "%s%s", boxIds[i], boxIds[i] + k + 1);
                    }
                }
            }
        }
        
        for (int i = 0; i < boxCount; i++) {
            free(boxIds[i]);
        }

        free(boxIds);
    }

    print(aoc, STRING, answer);

    return 0;
}
