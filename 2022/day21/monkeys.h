#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

enum OperationType {
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE
};

struct Operation {
    char leftMonkeyName[5];
    char operator;
    char rightMonkeyName[5];
};

struct Monkey {
    char name[5];
    long *number;
    struct Operation operation;
};

struct Monkey *findMonkey(char name[5], struct Monkey *monkeys, int monkeyCount) {
    for (int i = 0; i < monkeyCount; i++) {
        if (strcmp(monkeys[i].name, name) == 0) {
            return &monkeys[i];
        }
    }

    return NULL;
}

void playGame(struct Monkey *monkeys, int monkeyCount) {
    bool playing = true;

    while (playing) {
        bool monkeyUpdated = false;

        for (int i = 0; i < monkeyCount; i++) {
            if (!monkeys[i].number) {
                struct Monkey *leftMonkey = findMonkey(monkeys[i].operation.leftMonkeyName, monkeys, monkeyCount);
                struct Monkey *rightMonkey = findMonkey(monkeys[i].operation.rightMonkeyName, monkeys, monkeyCount);

                if (leftMonkey && leftMonkey->number && rightMonkey && rightMonkey->number) {
                    long leftMonkeyNumber = *leftMonkey->number;
                    long rightMonkeyNumber = *rightMonkey->number;

                    monkeys[i].number = (long *)calloc(1, sizeof(long));

                    switch (monkeys[i].operation.operator) {
                    case '+':
                        *monkeys[i].number = leftMonkeyNumber + rightMonkeyNumber;
                        break;
                    case '-':
                        *monkeys[i].number = leftMonkeyNumber - rightMonkeyNumber;
                        break;
                    case '*':
                        *monkeys[i].number = leftMonkeyNumber * rightMonkeyNumber;
                        break;
                    case '/':
                        *monkeys[i].number = leftMonkeyNumber / rightMonkeyNumber;
                        break;
                    }

                    monkeyUpdated = true;
                }
            }
        }

        if (!monkeyUpdated) {
            playing = false;
        }
    }
}

struct Monkey *getMonkeys(char *input, int *monkeyCount, char *skip) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        struct Monkey *monkeys = NULL;
        char line[32];
        char *job;

        *monkeyCount = 0;

        while (fgets(line, sizeof(line), inputFile)) {
            line[strcspn(line, "\n")] = 0;

            struct Monkey monkey = { "", NULL };

            job = strchr(line, ':');
            *job = 0;
            job += 2;

            strcpy(monkey.name, line);

            if (skip && strcmp(monkey.name, skip) == 0) {
                continue;
            }

            if (isdigit(*job)) {
                monkey.number = (long *)calloc(1, sizeof(long));
                sscanf(job, "%ld", monkey.number);
            } else {
                sscanf(job, "%s %c %s", monkey.operation.leftMonkeyName, &monkey.operation.operator, monkey.operation.rightMonkeyName);
            }

            if (monkeys) {
                monkeys = (struct Monkey *)realloc(monkeys, ((*monkeyCount) + 1) * sizeof(struct Monkey));
            } else {
                monkeys = (struct Monkey *)malloc(sizeof(struct Monkey));
            }

            monkeys[(*monkeyCount)++] = monkey;
        }

        fclose(inputFile);

        return monkeys;
    }

    return NULL;
}

void freeMonkeys(struct Monkey *monkeys, int monkeyCount) {
    for (int i = 0; i < monkeyCount; i++) {
        if (monkeys[i].number) {
            free(monkeys[i].number);
        }
    }

    free(monkeys);
}