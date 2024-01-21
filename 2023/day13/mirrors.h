#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define SIZE 32

int mirrorLocationScore(char pattern[SIZE][SIZE], int count, int multiplier, bool findingSmudge, int originalScore) {
    bool mirrorFound;
    int score = 0;

    if (findingSmudge) {
        int patternWidth = strlen(*pattern);

        for (int i = 0; !score && i < count; i++) {
            for (int j = 0; !score && j < patternWidth; j++) {
                char symbol = pattern[i][j];

                pattern[i][j] = symbol == '.' ? '#' : '.';

                score = mirrorLocationScore(pattern, count, multiplier, false, originalScore);

                pattern[i][j] = symbol;
            }
        }
    } else {
        for (int i = 1; !score && i < count; i++) {
            if (strcmp(pattern[i], pattern[i - 1]) == 0) {
                mirrorFound = true;

                for (int j = 1; mirrorFound && i + j < count && i - j - 1 >= 0; j++) {
                    mirrorFound = strcmp(pattern[i + j], pattern[i - j - 1]) == 0;
                }

                if (mirrorFound && i * multiplier != originalScore) {
                    score = i * multiplier;
                }
            }
        }
    }

    return score;
}

int mirrorLocationScoreSum(char *input, bool findSmudges) {
    FILE *inputFile = fopen(input, "r");
    int sum = 0;

    if (inputFile) {
        int rowCount = 0;
        int columnCount = 0;
        char patternRows[SIZE][SIZE];
        char patternColumns[SIZE][SIZE];
        bool mirrorFound;
        int score;

        while (!feof(inputFile)) {
            rowCount = 0;

            fgets(patternRows[rowCount], sizeof(patternRows[rowCount]), inputFile);
            patternRows[rowCount][strcspn(patternRows[rowCount], "\n")] = 0;

            columnCount = strlen(patternRows[rowCount]);

            for (int i = 0; i < columnCount; i++) {
                strcpy(patternColumns[i], "");
            }

            do {
                for (int i = 0; i < columnCount; i++) {
                    memmove(patternColumns[i] + 1, patternColumns[i], rowCount);
                    patternColumns[i][0] = patternRows[rowCount][i];
                    patternColumns[i][rowCount + 1] = '\0';
                }

                ++rowCount;

                fgets(patternRows[rowCount], sizeof(patternRows[rowCount]), inputFile);
                patternRows[rowCount][strcspn(patternRows[rowCount], "\n")] = 0;
            } while (!feof(inputFile) && strlen(patternRows[rowCount]) > 0);

            score = mirrorLocationScore(patternRows, rowCount, 100, false, 0);

            if (!score) {
                score = mirrorLocationScore(patternColumns, columnCount, 1, false, 0);
            }

            if (findSmudges) {
                int originalScore = score;

                score = mirrorLocationScore(patternRows, rowCount, 100, true, originalScore);
    
                if (!score) {
                    score = mirrorLocationScore(patternColumns, columnCount, 1, true, originalScore);
                }
            }

            sum += score;
        }

        fclose(inputFile);
    }

    return sum;
}