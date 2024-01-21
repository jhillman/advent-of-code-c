#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct Rule {
    char character;
    int **options;
};

struct CurrentRule {
    int number;
    struct CurrentRule *next;
};

struct RulesData {
    struct Rule *rules;
    int ruleCount;
    FILE *inputFile;
};

bool matchesRule(char *input, struct CurrentRule *currentRule, struct Rule *rules);
void freeRule(struct Rule *rule);
void freeCurrentRule(struct CurrentRule *currentRule);

struct CurrentRule *copyCurrentRule(struct CurrentRule *currentRule) {
    struct CurrentRule *currentRuleCopy = NULL;

    if (currentRule) {
        currentRuleCopy = (struct CurrentRule *) calloc(1, sizeof(struct CurrentRule *));
        currentRuleCopy->number = currentRule->number;

        if (currentRule->next) {
            currentRuleCopy->next = copyCurrentRule(currentRule->next);
        }
    }

    return currentRuleCopy;
}

bool matchesRule(char *input, struct CurrentRule *currentRule, struct Rule *rules) {
    if (strlen(input) == 0 && !currentRule) {
        return true;
    }

    if (strlen(input) == 0 || !currentRule) {
        freeCurrentRule(currentRule);

        return false;
    }

    struct Rule rule = rules[currentRule->number];
    struct CurrentRule *nextRule = currentRule->next;

    free(currentRule);
    
    if (rule.options) {
        bool optionsMatch = false;

        for (int i = 0; !optionsMatch && i < **rule.options; i++) {
            struct CurrentRule *optionRule =  (struct CurrentRule *) calloc(1, sizeof(struct CurrentRule *));
            struct CurrentRule *currentOptionRule = optionRule;

            for (int j = 0; j < *rule.options[i + 1]; j++) {
                if (j > 0) {
                    currentOptionRule->next = (struct CurrentRule *) calloc(1, sizeof(struct CurrentRule *));
                    currentOptionRule = currentOptionRule->next;
                }

                currentOptionRule->number = rule.options[i + 1][j + 1];
            }

            currentOptionRule->next = copyCurrentRule(nextRule);

            optionsMatch = matchesRule(input, optionRule, rules);
        }

        freeCurrentRule(nextRule);

        return optionsMatch;
    } else {
        if (*input == rule.character) {
            return matchesRule(input + 1, nextRule, rules);
        } else {
            return false;            
        }
    }

    return false;
}

int *parseOption(char *rule) {
    char ruleCopy[32];
    
    strcpy(ruleCopy, rule);
    char *rulePart = strtok(ruleCopy, " ");
    int optionLength = 0;

    while (rulePart) {
        ++optionLength;

        rulePart = strtok(NULL, " ");
    }

    int *option = (int *) calloc(optionLength + 1, sizeof(int));
    *option = optionLength;

    strcpy(ruleCopy, rule);
    rulePart = strtok(ruleCopy, " ");

    int optionIndex = 1;

    while (rulePart) {
        option[optionIndex++] = atoi(rulePart);

        rulePart = strtok(NULL, " ");
    }

    return option;
}

int **parseOptions(char *rule) {
    char ruleCopy[32];
    
    strcpy(ruleCopy, rule);
    char *rulePart = strtok(ruleCopy, "|");
    int *optionCount = (int *) calloc(1, sizeof(int));
    *optionCount = 0;

    while (rulePart) {
        ++*optionCount;

        rulePart = strtok(NULL, "|");
    }

    int **options = (int **) calloc(*optionCount + 1, sizeof(int **));
    *options = optionCount;

    strcpy(ruleCopy, rule);
    rulePart = strtok(ruleCopy, "|");

    char ruleParts[*optionCount][16];
    int rulePartIndex = 0;

    while (rulePart) {
        while (*rulePart == ' ') {
            ++rulePart;
        }

        strcpy(ruleParts[rulePartIndex++], rulePart);

        rulePart = strtok(NULL, "|");
    }

    int optionsIndex = 0;

    while (optionsIndex < *optionCount) {
        options[optionsIndex + 1] = parseOption(ruleParts[optionsIndex]);

        ++optionsIndex;
    }

    return options;
}

void parseRule(int number, char *ruleString, struct Rule *rules) {
    freeRule(&rules[number]);

    if (strstr(ruleString, "\"")) {
        rules[number].character = *(ruleString + 1);
    } else {
        rules[number].options = parseOptions(ruleString);
    }
}

struct RulesData *readRulesData(char *input) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        struct RulesData *data = (struct RulesData *) calloc(1, sizeof(struct RulesData));
        data->ruleCount = 0;

        char line[100];

        while (fgets(line, sizeof(line), inputFile) && strchr(line, ':')) {
            ++data->ruleCount;
        }

        fseek(inputFile, 0, SEEK_SET);

        data->rules = (struct Rule *) calloc(data->ruleCount, sizeof(struct Rule));
        data->inputFile = inputFile;

        int number;
        char *ruleString;
        char *rulePart;

        while (fgets(line, sizeof(line), inputFile) && (ruleString = strstr(line, ":"))) {
            sscanf(line, "%d", &number);
            ruleString += 2;

            parseRule(number, ruleString, data->rules);
        }

        return data;
    }

    return NULL;
}

int validInputCount(struct RulesData *data) {
    int validCount = 0;
    char input[100];

    while (fgets(input, sizeof(input), data->inputFile)) {
        input[strcspn(input, "\n")] = 0;

        struct CurrentRule *currentRule = (struct CurrentRule *) calloc(1, sizeof(struct CurrentRule *));
        currentRule->number = 0;

        if (matchesRule(input, currentRule, data->rules)) {
            ++validCount;
        }
    }

    return validCount;
}

void freeRule(struct Rule *rule) {
    if (rule->options) {
        for (int i = 0; i < **rule->options; i++) {
            free(rule->options[i + 1]);                
        }

        free(rule->options);
        rule->options = NULL;
    }
}

void freeCurrentRule(struct CurrentRule *currentRule) {
    if (currentRule) {
        freeCurrentRule(currentRule->next);

        free(currentRule);
    }
}

void freeRulesData(struct RulesData *data) {
    fclose(data->inputFile);

    for (int i = 0; i < data->ruleCount; i++) {
        freeRule(&data->rules[i]);
    }

    free(data->rules);
    free(data);
}