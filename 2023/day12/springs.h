#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

long validArrangementCount(char *springs, int *groups, int groupCount) {
    int characterCount = (int)strlen(springs);
    int groupBoolCount = 1;
    bool *groupBools;
    int groupBoolIndex = 0;
    long **arrangements;
    bool damaged;
    bool operational;
    long valid;

    for (int i = 0; i < groupCount; i++) {
        groupBoolCount += groups[i] + 1;
    }

    groupBools = (bool *)malloc(groupBoolCount * sizeof(bool));

    groupBools[groupBoolIndex++] = false;

    for (int i = 0; i < groupCount; i++) {
        for (int j = 0; j < groups[i]; j++) {
            groupBools[groupBoolIndex++] = true;
        }

        groupBools[groupBoolIndex++] = false;
    }

    arrangements = (long **)malloc((characterCount + 1) * sizeof(long *));

    for (int i = 0; i < characterCount + 1; i++) {
        arrangements[i] = (long *)calloc(groupBoolCount + 1, sizeof(long));
    }

    arrangements[characterCount][groupBoolCount] = 1;

    for (int i = characterCount - 1; i >= 0; i--) {
        for (int j = groupBoolCount - 1; j >= 0; j--) {
            if ((springs[i] == '#' || springs[i] =='?') && groupBools[j]) {
                arrangements[i][j] += arrangements[i + 1][j + 1];
            } else if ((springs[i] == '.' || springs[i] =='?') && !groupBools[j]) {
                arrangements[i][j] += arrangements[i + 1][j + 1] + arrangements[i + 1][j];
            }
        }
    }

    valid = arrangements[0][0];

    for (int i = 0; i < characterCount + 1; i++) {
        free(arrangements[i]);
    }

    free(arrangements);
    free(groupBools);

    return valid;
}

long arrangementCountSum(char *input, int copies) {
    FILE *inputFile = fopen(input, "r");
    long sum = 0;

    if (inputFile) {
        char data[64];
        char springs[32];
        char expandedSprings[128];
        char *groupNumber;
        int groups[64];
        int groupCount;

        while (fgets(data, sizeof(data), inputFile)) {
            data[strcspn(data, "\n")] = 0;

            strcpy(springs, strtok(data, " "));
            strcpy(expandedSprings, ".");
            strcpy(expandedSprings + 1, springs);
            groupNumber = strtok(strtok(NULL, " "), ",");
            groupCount = 0;

            while (groupNumber) {
                groups[groupCount++] = atoi(groupNumber);

                groupNumber = strtok(NULL, ",");
            }

            int startingSpringsLength = strlen(springs);
            int startingGroupCount = groupCount;

            for (int i = 1; i < copies; i++) {
                strcpy(expandedSprings + strlen(expandedSprings), "?");
                strcpy(expandedSprings + strlen(expandedSprings), springs);

                for (int j = 0; j < startingGroupCount; j++) {
                    groups[groupCount++] = groups[j];
                }
            }

            strcpy(expandedSprings + strlen(expandedSprings), ".");

            sum += validArrangementCount(expandedSprings, groups, groupCount);
        }

        fclose(inputFile);
    }

    return sum;
}