#include <stdlib.h>
#include <stdbool.h>

struct ProgramsIndex {
    char *programs;
    int index;
};

struct ProgramsList {
    int capacity;
    int count;
    struct ProgramsIndex *data;
};

int compare(const void *a, const void *b) {
    return strcmp(((struct ProgramsIndex *)a)->programs, ((struct ProgramsIndex *)b)->programs);
}

void addPrograms(struct ProgramsList *list, char *programs, int index) {
    if (list->count == list->capacity) {
        list->capacity += 5;
        list->data = (struct ProgramsIndex *)realloc(list->data, list->capacity * sizeof(struct ProgramsIndex));
    }

    struct ProgramsIndex programsIndex = { (char *)malloc(strlen(programs) + 1), index };
    strcpy(programsIndex.programs, programs);
    
    list->data[list->count++] = programsIndex;

    qsort(list->data, list->count, sizeof(struct ProgramsIndex), compare);
}

struct ProgramsIndex *findPrograms(struct ProgramsList *list, char* programs, int low, int high) {
    if (list->count == 0 || low > high) {
        return NULL;
    }

    int middle = low + ((high - low) / 2);

    if (middle < list->count) {
        int result = strcmp(programs, list->data[middle].programs);

        if (result == 0) {
            return &list->data[middle];
        } else if (result < 0) {
            return findPrograms(list, programs, low, middle - 1);
        } else {
            return findPrograms(list, programs, middle + 1, high);
        }
    }

    return NULL;
}

void freePrograms(struct ProgramsList *list) {
    for (int i = 0; i < list->count; i++) {
        free(list->data[i].programs);
    }

    free(list->data);
    free(list);
}

char *dance(char *input, struct ProgramsList *list, int moves) {
    FILE *inputFile = fopen(input, "r");
    char *result = NULL;

    if (inputFile) {
        char programs[] = "abcdefghijklmnop";
        int length = strlen(programs);
        char swap[17];
        char move[8];
        int sX;
        int xA;
        int xB;
        char pA;
        char pB;
        char *a;
        char *b;
        char s;
        bool cycleFound = false;
        struct ProgramsIndex *programsIndex;

        for (int i = 1; !cycleFound && i <= moves; i++) {
            while (!cycleFound && !feof(inputFile) && fscanf(inputFile, "%[^,]", move)) {
                if (!feof(inputFile)) {
                    fgetc(inputFile); // comma
                }

                switch (*move) {
                case 's':
                    sscanf(move, "s%d", &sX);
                    strncpy(swap, programs + length - sX, sX);
                    swap[sX] = '\0'; 
                    memcpy(programs + sX, programs, length - sX);
                    strncpy(programs, swap, sX);
                    break;
                case 'x':
                    sscanf(move, "x%d/%d", &xA, &xB);
                    s = programs[xA];
                    programs[xA] = programs[xB];
                    programs[xB] = s;
                    break;
                case 'p':
                    sscanf(move, "p%c/%c", &pA, &pB);
                    a = strchr(programs, pA);
                    b = strchr(programs, pB);
                    s = *a;
                    *a = *b;
                    *b = s;
                    break;
                }
            }

            fseek(inputFile, 0, SEEK_SET);

            if (list) {
                programsIndex = findPrograms(list, programs, 0, list->count);

                if (programsIndex && (moves - i) % (i - programsIndex->index) == 0) {
                    cycleFound = true;
                } else {
                    addPrograms(list, programs, i);
                }
            }
        }

        if (list) {
            freePrograms(list);
        }

        result = (char *)malloc(length + 1);
        strcpy(result, programs);

        fclose(inputFile);
    }

    return result;
}
