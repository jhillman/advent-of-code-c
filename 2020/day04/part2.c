/* Day 4, part 2 = 131 */

#include "../../aoc.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct YearRange {
    int min;
    int max;
};

bool yearValid(char *yearString, struct YearRange yearRange) {
    if (strlen(yearString) != 4) {
        return false;
    }

    int year;

    if (sscanf(yearString, "%d", &year) != 1) {
        return false;
    }

    return year >= yearRange.min && year <= yearRange.max;
}

bool heightValid(char *height) {
    int value;

    if (strstr(height, "cm") && sscanf(height, "%dcm", &value) == 1) {
        return value >= 150 && value <= 193;
    } else if (strstr(height, "in") && sscanf(height, "%din", &value) == 1) {
        return value >= 59 && value <= 76;
    }

    return false;
}

bool hairColorValid(char *hairColor) {
    if (strlen(hairColor) != 7 || *hairColor != '#') {
        return false;
    }

    char characters[7];
    char character;

    if (sscanf(hairColor + 1, "%s", characters) != 1) {
        return false;
    }

    bool valid = true;

    for (int i = 0; valid && i < 6; i++) {
        character = characters[i];

        valid = (character >= '0' && character <= '9') || (character >= 'a' && character <= 'f');
    }

    return true;
}

bool eyeColorValid(char *eyeColor) {
    if (strlen(eyeColor) != 3) {
        return false;
    }

    char *colors[] = { "amb", "blu", "brn", "gry", "grn", "hzl", "oth" };
    int colorCount = sizeof(colors) / sizeof(*colors);
    bool valid = false;

    for (int i = 0; !valid && i < colorCount; i++) {
        if (strcmp(eyeColor, colors[i]) == 0) {
            valid = true;
        }
    }

    return valid;
}

bool passportIdValid(char *passportIdString) {
    if (strlen(passportIdString) != 9) {
        return false;
    }

    int passportId;

    if (sscanf(passportIdString, "%9d", &passportId) == 1) {
        return true;
    }

    return false;
}

bool allThereAndValid(int keyValuePairsCount, char keyValuePairs[][20]) {
    struct YearRange birthYearRange = { 1920, 2002 };
    struct YearRange issueYearRange = { 2010, 2020 };
    struct YearRange expirationYearRange = { 2020, 2030 };

    char *requiredKeys[] = { "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid" };
    int requiredKeyCount = sizeof(requiredKeys) / sizeof(*requiredKeys);
    char keyValueSeparator[] = ":";
    char *key;
    char *value;
    bool valid = true;
    char allKeys[32] = "";

    for (int i = 0; valid && i < keyValuePairsCount; i++) {
        if ((key = strtok(keyValuePairs[i], keyValueSeparator))) {
            strcpy(allKeys + strlen(allKeys), key);

            value = strtok(NULL, keyValueSeparator);

            if (strcmp(key, "byr") == 0) {
                valid = yearValid(value, birthYearRange);
            } else if (strcmp(key, "iyr") == 0) {
                valid = yearValid(value, issueYearRange);
            } else if (strcmp(key, "eyr") == 0) {
                valid = yearValid(value, expirationYearRange);
            } else if (strcmp(key, "hgt") == 0) {
                valid = heightValid(value);
            } else if (strcmp(key, "hcl") == 0) {
                valid = hairColorValid(value);
            } else if (strcmp(key, "ecl") == 0) {
                valid = eyeColorValid(value);
            } else if (strcmp(key, "pid") == 0) {
                valid = passportIdValid(value);
            }
        }
    }

    for (int i = 0; valid && i < requiredKeyCount; i++) {
        if (!strstr(allKeys, requiredKeys[i])) {
            valid = false;
        }
    }

    return valid;
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
                if (allThereAndValid(keyValuePairsCount, keyValuePairs)) {
                    ++validCount;
                }

                keyValuePairsCount = 0;
            } else {
                char keyValuePairSeparator[] = " ";
                char *keyValuePtr = strtok(dataLine, keyValuePairSeparator);

                while (keyValuePtr) {
                    if (!strstr(keyValuePtr, "cid")) {
                        strcpy(keyValuePairs[keyValuePairsCount++], keyValuePtr);
                    }

                    keyValuePtr = strtok(NULL, keyValuePairSeparator);                
                }
            }
        }

        fclose(inputFile);

        if (keyValuePairsCount > 0) {
            if (allThereAndValid(keyValuePairsCount, keyValuePairs)) {
                ++validCount;
            }
        }

        answer = validCount;
    }

    print(aoc, INT, &answer);

    return 0;
}