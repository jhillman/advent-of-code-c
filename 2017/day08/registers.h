#include <stdlib.h>
#include <stdbool.h>

struct Register {
    char name[4];
    int value;
    int maxValue;
};

struct Registers {
    int capacity;
    int count;
    struct Register **data;
};

enum DeltaType {
    INC,
    DEC
};

enum Condition {
    EQ,
    NEQ,
    LT,
    LTE,
    GT,
    GTE
};

enum MaxType {
    END,
    OVERALL
};

int compare(const void *a, const void *b) {
    struct Register *regA = *(struct Register **)a;
    struct Register *regB = *(struct Register **)b;

    return strcmp(regA->name, regB->name);
}

struct Register *addRegister(struct Registers *registers, char *name) {
    if (registers->count == registers->capacity) {
        registers->capacity += 100;
        registers->data = (struct Register **)realloc(registers->data, registers->capacity * sizeof(struct Register *));
    }

    struct Register *reg = (struct Register *)calloc(1, sizeof(struct Register));
    strcpy(reg->name, name);

    registers->data[registers->count++] = reg;

    qsort(registers->data, registers->count, sizeof(struct Register *), compare);

    return reg;
}

struct Register *findRegister(struct Registers *registers, char* name, int low, int high) {
    if (registers->count == 0 || low > high) {
        return NULL;
    }

    int middle = low + ((high - low) / 2);

    if (middle < registers->count) {
        int result = strcmp(name, registers->data[middle]->name);

        if (result == 0) {
            return registers->data[middle];
        } else if (result < 0) {
            return findRegister(registers, name, low, middle - 1);
        } else {
            return findRegister(registers, name, middle + 1, high);
        }
    }

    return NULL;
}

struct Register *getRegister(struct Registers *registers, char *name) {
    struct Register *reg = findRegister(registers, name, 0, registers->count);

    if (!reg) {
        reg = addRegister(registers, name);
    }

    return reg;
}

enum DeltaType getDeltaType(char *string) {
    if (strcmp(string, "inc") == 0) {
        return INC;
    } else {
        return DEC;
    }
}

enum Condition getCondition(char *string) {
    if (strcmp(string, "==") == 0) {
        return EQ;
    } else if (strcmp(string, "!=") == 0) {
        return NEQ;
    } else if (strcmp(string, "<") == 0) {
        return LT;
    } else if (strcmp(string, "<=") == 0) {
        return LTE;
    } else if (strcmp(string, ">") == 0) {
        return GT;
    } else {
        return GTE;
    }
}

int maxRegister(char *input, enum MaxType type) {
    FILE *inputFile = fopen(input, "r");
    int max = 0;

    if (inputFile) {
        struct Registers *registers = (struct Registers *)calloc(1, sizeof(struct Registers));
        char targetRegisterName[4];
        char deltaTypeString[4];
        int delta;
        char conditionRegisterName[4];
        char conditionString[3];
        int conditionValue;
        struct Register *targetRegister;
        enum DeltaType deltaType;
        struct Register *conditionRegister;
        enum Condition condition;
        bool conditionMet;

        while (!feof(inputFile) && fscanf(inputFile, "%s %s %d if %s %s %d", 
            targetRegisterName, deltaTypeString, &delta, 
            conditionRegisterName, conditionString, &conditionValue)) {
            targetRegister = getRegister(registers, targetRegisterName);
            deltaType = getDeltaType(deltaTypeString);
            conditionRegister = getRegister(registers, conditionRegisterName);
            condition = getCondition(conditionString);

            conditionMet = false;

            switch (condition) {
            case EQ:
                conditionMet = conditionRegister->value == conditionValue;
                break;
            case NEQ:
                conditionMet = conditionRegister->value != conditionValue;
                break;
            case LT:
                conditionMet = conditionRegister->value < conditionValue;
                break;
            case LTE:
                conditionMet = conditionRegister->value <= conditionValue;
                break;
            case GT:
                conditionMet = conditionRegister->value > conditionValue;
                break;
            case GTE:
                conditionMet = conditionRegister->value >= conditionValue;
                break;
            }

            if (conditionMet) {
                switch (deltaType) {
                case INC:
                    targetRegister->value += delta;
                    break;
                case DEC:
                    targetRegister->value -= delta;
                    break;
                }
            }

            if (targetRegister->value > targetRegister->maxValue) {
                targetRegister->maxValue = targetRegister->value;
            }
        }

        fclose(inputFile);

        for (int i = 0; i < registers->count; i++) {
            switch (type) {
            case END:
                if (registers->data[i]->value > max) {
                    max = registers->data[i]->value;
                }
                break;
            case OVERALL:
                if (registers->data[i]->maxValue > max) {
                    max = registers->data[i]->maxValue;
                }
                break;
            }

            free(registers->data[i]);
        }

        free(registers);
    }

    return max;
}
