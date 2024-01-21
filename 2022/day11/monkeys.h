#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

enum Part {
    ONE,
    TWO
};

struct Item {
    long worryLevel;
    struct Item *next;
};

enum OperationType {
    MULTIPLY_VALUE,
    ADD_VALUE,
    MULTIPLY_SELF,
    ADD_SELF
};

struct Operation {
    enum OperationType type;
    int value;
};

struct Monkey {
    int number;
    struct Item *items;
    struct Item *lastItem;
    struct Operation operation;
    long divisibleTest;
    int trueMonkey;
    int falseMonkey;
    int inspectionCount;
};

struct Monkey *getMonkeys(char *input, int *monkeyCount) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        char line[64];
        *monkeyCount = 0;

        while (fgets(line, sizeof(line), inputFile)) {
            if (strstr(line, "Monkey") == line) {
                ++*monkeyCount;
            }
        }

        fseek(inputFile, 0, SEEK_SET);

        struct Monkey *monkeys = (struct Monkey *)calloc(*monkeyCount, sizeof(struct Monkey));
        char *items;
        char operator;
        char *operand;
        int value;

        for (int number = 0; number < *monkeyCount; number++) {
            struct Monkey monkey = {
                number, NULL, NULL, { MULTIPLY_VALUE, -1 }, 0, 0, 0, 0
            };

            fgets(line, sizeof(line), inputFile);

            fgets(line, sizeof(line), inputFile);
            items = strchr(line, ':') + 1;
            long worryLevel;
            char comma;

            while (items && sscanf(items, "%ld%c", &worryLevel, &comma) >= 1) {
                struct Item *item = (struct Item *)calloc(1, sizeof(struct Item));
                item->worryLevel = worryLevel;

                if (monkey.lastItem) {
                    monkey.lastItem->next = item;
                    monkey.lastItem = item;
                } else {
                    monkey.items = monkey.lastItem = item;
                }

                items = strchr(items, ',');

                if (items) {
                    items += 2;
                }
            }

            fgets(line, sizeof(line), inputFile);

            if (sscanf(line, "  Operation: new = old %c", &operator)) {
                operand = strchr(line, operator) + 2;

                if (isdigit(*operand)) {
                    monkey.operation.type = operator == '*' ? MULTIPLY_VALUE : ADD_VALUE;
                    sscanf(operand, "%d", &monkey.operation.value);
                } else {
                    monkey.operation.type = operator == '*' ? MULTIPLY_SELF : ADD_SELF;
                }
            }

            fgets(line, sizeof(line), inputFile);
            sscanf(line, "  Test: divisible by %ld", &monkey.divisibleTest);

            fgets(line, sizeof(line), inputFile);
            sscanf(line, "    If true: throw to monkey %d", &monkey.trueMonkey);

            fgets(line, sizeof(line), inputFile);
            sscanf(line, "    If false: throw to monkey %d", &monkey.falseMonkey);

            monkeys[number] = monkey;

            fgets(line, sizeof(line), inputFile);
        }

        fclose(inputFile);

        return monkeys;
    }

    return NULL;
}

void printMonkeys(struct Monkey *monkeys, int monkeyCount) {
    for (int i = 0; i < monkeyCount; i++) {
        struct Monkey monkey = monkeys[i];

        printf("Monkey: %d\n", monkey.number);
        printf("Items: ");
        struct Item *item = monkey.items;

        while (item) {
            printf("%ld%c", item->worryLevel, item->next ? ',' : ' ');

            item = item->next;
        }

        printf("\nInspection count: %d\n\n", monkey.inspectionCount);
    }
}

void throw(struct Item *item, struct Monkey *monkey) {
    item->next = NULL;

    if (monkey->lastItem) {
        monkey->lastItem->next = item;
        monkey->lastItem = item;
    } else {
        monkey->items = monkey->lastItem = item;
    }
}

long monkeyBusiness(struct Monkey *monkeys, int monkeyCount, int rounds, enum Part part) {
    int combinedTest = 1;

    if (part == TWO) {
        for (int i = 0; i < monkeyCount; i++) {
            combinedTest *= monkeys[i].divisibleTest;
        }
    }

    for (int round = 0; round < rounds; round++) {
        for (int i = 0; i < monkeyCount; i++) {
            struct Item *item = monkeys[i].items;
            struct Item *next;

            while (item) {
                next = item->next;

                ++monkeys[i].inspectionCount;

                switch (monkeys[i].operation.type) {
                case MULTIPLY_VALUE:
                    item->worryLevel *= monkeys[i].operation.value;
                    break;
                case ADD_VALUE:
                    item->worryLevel += monkeys[i].operation.value;
                    break;
                case MULTIPLY_SELF:
                    item->worryLevel *= item->worryLevel;
                    break;
                case ADD_SELF:
                    item->worryLevel += item->worryLevel;
                    break;
                }

                switch (part) {
                case ONE:
                    item->worryLevel /= 3;
                    break;
                case TWO:
                    item->worryLevel %= combinedTest;
                    break;
                }

                if (item->worryLevel % monkeys[i].divisibleTest == 0) {
                    throw(item, &monkeys[monkeys[i].trueMonkey]);
                } else {
                    throw(item, &monkeys[monkeys[i].falseMonkey]);
                }

                monkeys[i].items = monkeys[i].lastItem = NULL;

                item = next;
            }
        }
    }

    // printMonkeys(monkeys, monkeyCount);

    long countOne = 0;
    long countTwo = 0;

    for (int i = 0; i < monkeyCount; i++) {
        if (monkeys[i].inspectionCount > countOne) {
            countTwo = countOne;
            countOne = monkeys[i].inspectionCount;
        } else if (monkeys[i].inspectionCount > countTwo) {
            countTwo = monkeys[i].inspectionCount;
        }
    }

    return countOne * countTwo;
}

void freeMonkeys(struct Monkey *monkeys, int monkeyCount) {
    struct Item *item;
    struct Item *next;

    for (int i = 0; i < monkeyCount; i++) {
        item = monkeys[i].items;

        while (item) {
            next = item->next;
            free(item);

            item = next;
        }
    }

    free(monkeys);
}