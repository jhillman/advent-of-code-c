/* Day 4, part 1 = 210 */

#include "../../aoc.h"
#include <string.h>
#include <stdbool.h>

bool allThere(int keyValuePairsCount, char keyValuePairs[][20]) {
    char *requiredKeys[] = { "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid" };
    int requiredKeyCount = sizeof(requiredKeys) / sizeof(*requiredKeys);
    char keyValueSeparator[] = ":";
    char *key;
    char allKeys[32] = "";

    for (int i = 0; i < keyValuePairsCount; i++) {
        if ((key = strtok(keyValuePairs[i], keyValueSeparator))) {
            strcpy(allKeys + strlen(allKeys), key);
        }
    }

    bool allThere = true;

    for (int i = 0; allThere && i < requiredKeyCount; i++) {
        if (!strstr(allKeys, requiredKeys[i])) {
            allThere = false;
        }
    }

    return allThere;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    FILE *inputFile = fopen(aoc.input, "r");
    int answer = 0;

    if (inputFile) {
        char dataLine[100];
        char dataLineLength;
        char keyValuePairs[10][20];
        int keyValuePairsCount = 0;
        int validCount = 0;

        while (fgets(dataLine, sizeof(dataLine), inputFile)) {
            dataLine[strcspn(dataLine, "\n")] = 0;
            dataLineLength = strlen(dataLine);

            if (dataLineLength == 0) {
                if (allThere(keyValuePairsCount, keyValuePairs)) {
                    ++validCount;
                }

                keyValuePairsCount = 0;
            } else {
                char keyValuePairSeparator[] = " ";
                char *keyValuePtr = strtok(dataLine, keyValuePairSeparator);

                while (keyValuePtr) {
                    strcpy(keyValuePairs[keyValuePairsCount++], keyValuePtr);

                    keyValuePtr = strtok(NULL, keyValuePairSeparator);                
                }
            }
        }

        fclose(inputFile);

        if (keyValuePairsCount > 0) {
            if (allThere(keyValuePairsCount, keyValuePairs)) {
                ++validCount;
            }
        }

        answer = validCount;
    }

    print(aoc, INT, &answer);

    return 0;
}