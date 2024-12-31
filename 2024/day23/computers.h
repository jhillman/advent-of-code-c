#include <stdlib.h>
#include <stdbool.h>

struct Computers;

struct Computer {
    char name[3];
    struct Computers *connections;
};

int compareComputers(const void *a, const void *b) {
    return strcmp((*(struct Computer **)a)->name, (*(struct Computer **)b)->name);
}

struct Computers {
    int capacity;
    int count;
    struct Computer **data;
};

struct Computer *findComputer(struct Computers *computers, char *name, int low, int high) {
    if (computers->count == 0 || low > high) {
        return NULL;
    }

    int middle = low + ((high - low) / 2);

    if (middle < computers->count) {
        int result = strcmp(name, computers->data[middle]->name);

        if (result == 0) {
            return computers->data[middle];
        } else if (result < 0) {
            return findComputer(computers, name, low, middle - 1);
        } else {
            return findComputer(computers, name, middle + 1, high);
        }
    }

    return NULL;
}

void addComputer(struct Computers *computers, struct Computer *computer, bool check, bool sort) {
    if (check && findComputer(computers, computer->name, 0, computers->count)) {
        return;
    }

    if (computers->count == computers->capacity) {
        computers->capacity += 100;
        computers->data = (struct Computer **)realloc(computers->data, computers->capacity * sizeof(struct Computer *));
    }

    computers->data[computers->count++] = computer;

    if (sort) {
       qsort(computers->data, computers->count, sizeof(struct Computer *), compareComputers);
    }
}

struct Computer *getComputer(struct Computers *computers, char *name) {
    struct Computer *computer = findComputer(computers, name, 0, computers->count);

    if (computer == NULL) {
        computer = (struct Computer *)malloc(sizeof(struct Computer));
        strcpy(computer->name, name);
        computer->connections = (struct Computers *)calloc(1, sizeof(struct Computers));

        addComputer(computers, computer, false, true);
    }

    return computer;
}

struct Computers *getComputers(char *input) {
    FILE *inputFile = fopen(input, "r");
    struct Computers *computers = NULL;

    if (inputFile) {
        char connection[8];
        struct Computer *computer1;
        struct Computer *computer2;

        computers = (struct Computers *)calloc(1, sizeof(struct Computers));

        while (!feof(inputFile) && fgets(connection, sizeof(connection), inputFile)) {
            connection[strcspn(connection, "\n")] = 0;

            computer1 = getComputer(computers, strtok(connection, "-"));
            computer2 = getComputer(computers, strtok(NULL, "-"));

            addComputer(computer1->connections, computer2, false, true);
            addComputer(computer2->connections, computer1, false, true);
        }

        fclose(inputFile);
    }

    return computers;
}

void freeComputers(struct Computers *computers, bool freeData) {
    if (freeData) {
        for (int i = 0; i < computers->count; i++) {
            freeComputers(computers->data[i]->connections, false);
            free(computers->data[i]);
        }
    }

    free(computers->data);
    free(computers);
}

struct String {
    char *value;
    int count;
};

struct Strings {
    int capacity;
    int count;
    struct String **data;
};

int compareStrings(const void *a, const void *b) {
    return strcmp((*(struct String **)a)->value, (*(struct String **)b)->value);
}

struct String *findString(struct Strings *strings, char *value, int low, int high) {
    if (strings->count == 0 || low > high) {
        return NULL;
    }

    int middle = low + ((high - low) / 2);

    if (middle < strings->count) {
        int result = strcmp(value, strings->data[middle]->value);

        if (result == 0) {
            return strings->data[middle];
        } else if (result < 0) {
            return findString(strings, value, low, middle - 1);
        } else {
            return findString(strings, value, middle + 1, high);
        }
    }

    return NULL;
}

struct String *addString(struct Strings *strings, char *value, bool copy, bool sort) {
    struct String *string = findString(strings, value, 0, strings->count);

    if (string) {
        return string;
    }

    if (strings->count == strings->capacity) {
        strings->capacity += 100;
        strings->data = (struct String **)realloc(strings->data, strings->capacity * sizeof(struct String *));
    }

    string = (struct String *)calloc(1, sizeof(struct String));

    if (copy) {
        string->value = (char *)malloc(strlen(value) + 1);
        strcpy(string->value, value);
    } else {
        string->value = value;
    }

    strings->data[strings->count++] = string;

    if (sort) {
        qsort(strings->data, strings->count, sizeof(struct String *), compareStrings);
    }

    return string;
}

struct String *getString(struct Strings *strings, char *value) {
    struct String *string = findString(strings, value, 0, strings->count);

    if (string == NULL) {
        string = addString(strings, value, true, true);
    }

    return string;
}

void freeStrings(struct Strings *strings, bool freeData) {
    for (int i = 0; i < strings->count; i++) {
        if (freeData) {
            free(strings->data[i]->value);
        }

        free(strings->data[i]);
    }

    free(strings->data);
    free(strings);
}
