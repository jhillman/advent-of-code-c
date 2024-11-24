#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool memoryEqual(int *first, int *second, int bankCount) {
    bool equal = true;

    for (int i = 0; equal && i < bankCount; i++) {
        equal = first[i] == second[i];
    }

    return equal;
}

void redistribute(int *memory, int bankCount) {
    int value = 0;
    int bankIndex;

    for (int i = 0; i < bankCount; i++) {
        if (memory[i] > value) {
            value = memory[i];
            bankIndex = i;
        }
    }

    memory[bankIndex] = 0;

    while (value > 0) {
        if (++bankIndex == bankCount) {
            bankIndex = 0;
        }

        ++memory[bankIndex];
        --value;
    }
}

int reallocate(char *input, int part) {
    FILE *inputFile = fopen(input, "r");
    int answer = 0;

    if (inputFile) {
        int value;
        int bankCount = 0;
        int *memory;
        int *tortoise;
        int *hare;
        int position = 0;
        int cycle = 1;

        while (!feof(inputFile) && fscanf(inputFile, "%d", &value)) {
            ++bankCount;
        }

        memory = (int *)malloc(bankCount * sizeof(int));
        tortoise = (int *)malloc(bankCount * sizeof(int));
        hare = (int *)malloc(bankCount * sizeof(int));

        bankCount = 0;
        fseek(inputFile, 0, SEEK_SET);

        while (!feof(inputFile) && fscanf(inputFile, "%d", &value)) {
            memory[bankCount] = value;
            tortoise[bankCount] = value;
            hare[bankCount++] = value;
        }

        fclose(inputFile);

        // https://en.wikipedia.org/wiki/Cycle_detection#Floyd.27s_Tortoise_and_Hare
        redistribute(tortoise, bankCount);
        redistribute(hare, bankCount);
        redistribute(hare, bankCount);

        while (!memoryEqual(tortoise, hare, bankCount)) {
            redistribute(tortoise, bankCount);
            redistribute(hare, bankCount);
            redistribute(hare, bankCount);
        }

        memcpy(tortoise, memory, bankCount * sizeof(int));

        while (!memoryEqual(tortoise, hare, bankCount)) {
            redistribute(tortoise, bankCount);
            redistribute(hare, bankCount);
            ++position;
        }

        memcpy(hare, tortoise, bankCount * sizeof(int));
        redistribute(hare, bankCount);

        while (!memoryEqual(tortoise, hare, bankCount)) {
            redistribute(hare, bankCount);
            ++cycle;
        }

        free(memory);
        free(tortoise);
        free(hare);

        if (part == 1) {
            return position + cycle;
        } else {
            return cycle;
        }
    }

    return -1;
}
