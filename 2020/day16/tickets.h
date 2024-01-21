#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct Range {
    int min;
    int max;
};

struct Rule {
    char name[20];
    struct Range rangeOne;
    struct Range rangeTwo;
    int valueIndex;
};

struct Value {
    bool valid;
    int value;
};

struct Ticket {
    int valid;
    struct Value *values;
};

struct TicketData {
    int ruleCount;
    struct Rule *rules;
    int valueCount;
    struct Ticket mine;
    int nearbyCount;
    struct Ticket *nearby;
};

int valueValidForRule(int value, struct Rule rule) {
    if ((value >= rule.rangeOne.min && value <= rule.rangeOne.max) || 
        (value >= rule.rangeTwo.min && value <= rule.rangeTwo.max)) {
        return 1;
    }

    return 0;
}

struct TicketData *getTicketData(char *input) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        struct TicketData *data = (struct TicketData *) calloc(1, sizeof(struct TicketData));

        char line[90];
        data->ruleCount = 0;

        while (fgets(line, 90, inputFile) && strlen(line) > 1) {
            ++data->ruleCount;
        }

        fseek(inputFile, 0, SEEK_SET);

        data->rules = (struct Rule *) calloc(data->ruleCount, sizeof(struct Rule));

        int ruleIndex = 0;
        char *charPtr;
        int value;
        char ch;

        while (fgets(line, 90, inputFile) && strlen(line) > 1) {
            if ((charPtr = strstr(line, ":"))) {
                strncpy(data->rules[ruleIndex].name, line, charPtr - line);
                data->rules[ruleIndex].name[charPtr - line] = '\0';

                sscanf(charPtr + 2, "%d-%d or %d-%d", 
                    &data->rules[ruleIndex].rangeOne.min, 
                    &data->rules[ruleIndex].rangeOne.max, 
                    &data->rules[ruleIndex].rangeTwo.min, 
                    &data->rules[ruleIndex].rangeTwo.max);
            }

            data->rules[ruleIndex].valueIndex = -1;

            ++ruleIndex;
        }

        while (fgets(line, 90, inputFile) && !strstr(line, "your ticket:"));
        fgets(line, 90, inputFile);

        data->valueCount = 0;
        charPtr = line;

        while (*charPtr) {
            if (*charPtr == ',') {
                ++data->valueCount;
            }

            ++charPtr;
        }

        if (*charPtr != ',' && data->valueCount > 0) {
            ++data->valueCount;
        }

        data->mine.values = (struct Value *) calloc(data->valueCount, sizeof(struct Value));
        data->mine.valid = 1;

        int valueIndex = 0;

        charPtr = strtok(line, ",");

        while (charPtr) {
            value = atoi(charPtr);
            data->mine.values[valueIndex].value = value;

            for (int i = 0; !data->mine.values[valueIndex].valid && i < data->ruleCount; i++) {
                if (valueValidForRule(value, data->rules[i])) {
                    data->mine.values[valueIndex].valid = 1;
                }
            }

            if (!data->mine.values[valueIndex].valid) {
                data->mine.valid = 0;
            }

            ++valueIndex;

            charPtr = strtok(NULL, ",");
        }

        while (fgets(line, 90, inputFile) && !strstr(line, "nearby tickets:"));

        int nearbyTicketLocation = ftell(inputFile);
        data->nearbyCount = 0;

        while (fgets(line, 90, inputFile)) {
            ++data->nearbyCount ;
        }

        fseek(inputFile, nearbyTicketLocation, SEEK_SET);

        data->nearby = (struct Ticket *) calloc(data->nearbyCount, sizeof(struct Ticket));
        int nearbyIndex = 0;

        while (fgets(line, 90, inputFile)) {
            data->nearby[nearbyIndex].values = (struct Value *) calloc(data->valueCount, sizeof(struct Value));
            data->nearby[nearbyIndex].valid = 1;

            valueIndex = 0;
            charPtr = strtok(line, ",");

            while (charPtr) {
                value = atoi(charPtr);
                data->nearby[nearbyIndex].values[valueIndex].value = value;

                for (int i = 0; !data->nearby[nearbyIndex].values[valueIndex].valid && i < data->ruleCount; i++) {
                    if (valueValidForRule(value, data->rules[i])) {
                        data->nearby[nearbyIndex].values[valueIndex].valid = 1;
                    }
                }

                if (!data->nearby[nearbyIndex].values[valueIndex].valid) {
                    data->nearby[nearbyIndex].valid = 0;
                }

                ++valueIndex;

                charPtr = strtok(NULL, ",");
            }

            ++nearbyIndex;
        }

        fclose(inputFile);

        return data;
    }

    return NULL;
}

void freeTicketData(struct TicketData *data) {
    free(data->rules);
    free(data->mine.values);

    for (int i = 0; i < data->nearbyCount; i++) {
        free(data->nearby[i].values);
    }

    free(data->nearby);

    free(data);
}