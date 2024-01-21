#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int compare(const void *a, const void *b) {
    return *(int *)b - *(int *)a;
}

int *readElfCalories(char *input) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        char line[8];
        int calories = 0;
        int elfCapacity = 50;
        int elfIndex = 0;
        int *elfCalories = (int *)malloc(elfCapacity * sizeof(int));

        while (fgets(line, sizeof(line), inputFile)) {
            if (elfIndex == elfCapacity) {
                elfCapacity += 50;

                elfCalories = (int *)realloc(elfCalories, elfCapacity * sizeof(int));
            }

            if (strlen(line) == 1) {
                ++elfIndex;

                elfCalories[elfIndex] = 0;
            } else {
                sscanf(line, "%d", &calories);

                elfCalories[elfIndex] += calories;
            }
        }

        fclose(inputFile);

        qsort(elfCalories, elfIndex, sizeof(int), compare);

        return elfCalories;
    }

    return NULL;    
}
