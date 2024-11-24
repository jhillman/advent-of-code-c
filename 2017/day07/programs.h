#include <stdlib.h>

struct Programs;

struct Program {
    char name[8];
    int weight;
    struct Program *holder;
    struct Programs *programs;
};

struct Programs {
    int capacity;
    int count;
    struct Program **data;
    struct Program *root;
};

int compare(const void *a, const void *b) {
    struct Program *programA = *(struct Program **)a;
    struct Program *programB = *(struct Program **)b;

    return strcmp(programA->name, programB->name);
}

struct Program *addProgram(struct Programs *programs, char *name) {
    if (programs->count == programs->capacity) {
        programs->capacity += 100;
        programs->data = (struct Program **)realloc(programs->data, programs->capacity * sizeof(struct Program *));
    }

    struct Program *program = (struct Program *)calloc(1, sizeof(struct Program));
    strcpy(program->name, name);

    programs->data[programs->count++] = program;

    qsort(programs->data, programs->count, sizeof(struct Program *), compare);

    return program;
}

struct Program *addSubProgram(struct Programs *programs, struct Program *program) {
    if (programs->count == programs->capacity) {
        programs->capacity += 5;
        programs->data = (struct Program **)realloc(programs->data, programs->capacity * sizeof(struct Program *));
    }

    programs->data[programs->count++] = program;

    return program;
}

struct Program *findProgram(struct Programs *programs, char* name, int low, int high) {
    if (programs->count == 0 || low > high) {
        return NULL;
    }

    int middle = low + ((high - low) / 2);

    if (middle < programs->count) {
        int result = strcmp(name, programs->data[middle]->name);

        if (result == 0) {
            return programs->data[middle];
        } else if (result < 0) {
            return findProgram(programs, name, low, middle - 1);
        } else {
            return findProgram(programs, name, middle + 1, high);
        }
    }

    return NULL;
}

struct Programs *getPrograms(char *input) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        char line[128];
        char name[8];
        int weight;
        char *subNames;
        char *subName;
        struct Program *program;
        struct Program *subProgram;
        struct Programs *programs = (struct Programs *)calloc(1, sizeof(struct Programs));

        while (fgets(line, sizeof(line), inputFile)) {
            line[strcspn(line, "\n")] = 0;

            sscanf(line, "%s (%d)", name, &weight);

            program = findProgram(programs, name, 0, programs->count);

            if (!program) {
                program = addProgram(programs, name);
            }

            program->weight = weight;

            if ((subNames = strstr(line, "->"))) {
                program->programs = (struct Programs *)calloc(1, sizeof(struct Programs));

                subName = strtok(subNames + 3, " ,");

                while (subName) {
                    subProgram = findProgram(programs, subName, 0, programs->count);

                    if (!subProgram) {
                        subProgram = addProgram(programs, subName);
                    }

                    subProgram->holder = program;
                    addSubProgram(program->programs, subProgram);

                    subName = strtok(NULL, " ,");
                }
            }
        }

        fclose(inputFile);

        for (int i = 0; i < programs->count; i++) {
            if (programs->data[i]->holder == NULL) {
                programs->root = programs->data[i];
                break;
            }
        }

        return programs;
    }

    return NULL;
}

void freePrograms(struct Programs *programs) {
    for (int i = 0; i < programs->count; i++) {
        if (programs->data[i]->programs) {
            free(programs->data[i]->programs->data);
            free(programs->data[i]->programs);
        }

        free(programs->data[i]);
    }

    free(programs->data);
    free(programs);
}