#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

struct CrabData {
    int crabCount;
    long *positions;
    long minPosition;
    long maxPosition;
};

struct CrabData *getCrabData(char *input) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        struct CrabData *data = (struct CrabData *)calloc(1, sizeof(struct CrabData));

        long position;
        char ch;
        
        data->crabCount = 0;
        data->minPosition = 0;
        data->maxPosition = 10000;

        while (fscanf(inputFile, "%ld%c", &position, &ch) >= 1) {
            ++data->crabCount;
        }

        data->positions = (long *)calloc(data->crabCount, sizeof(long));

        fseek(inputFile, 0, SEEK_SET);

        for (int i = 0; i < data->crabCount; i++) {
            fscanf(inputFile, "%ld%c", &position, &ch);

            data->positions[i] = position;

            data->minPosition = min(data->minPosition, position);
            data->maxPosition = max(data->maxPosition, position);
        }

        fclose(inputFile);

        return data;
    }

    return NULL;
}

void freeCrabData(struct CrabData *data) {
    free(data->positions);
    free(data);
}