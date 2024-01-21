#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

enum ExtrapolationType {
    NEXT,
    PREVIOUS
};

struct Values {
    int count;
    long *data;
    long value;
};

long extrapolatedValue(struct Values values, enum ExtrapolationType type) {
    struct Values differences = { values.count - 1, malloc((values.count - 1) * sizeof(long)), 0 };
    bool allZero = true;

    for (int i = 0; i < values.count - 1; i++) {
        differences.data[i] = values.data[i + 1] - values.data[i];
        allZero = allZero && differences.data[i] == 0;
    }

    switch (type) {
    case NEXT:
        differences.value = differences.data[differences.count -1];
        break;
    case PREVIOUS:
        differences.value = differences.data[0];
        break;
    }

    if (allZero) {
        free(differences.data);

        return values.value;
    } else {
        long extrapolated = extrapolatedValue(differences, type);

        if (type == PREVIOUS) {
            extrapolated *= -1;
        }

        free(differences.data);

        return values.value + extrapolated;
    }
}

long getExtrapolatedSum(char *input, enum ExtrapolationType type) {
    FILE *inputFile = fopen(input, "r");
    long sum = 0;

    if (inputFile) {
        char c;
        struct Values values = { 0, NULL, 0 };

        do {
            c = fgetc(inputFile);
    
            ++values.count;

            while (isdigit(c)) {
                c = fgetc(inputFile);
            } 
        } while (c != '\n');

        values.data = (long *)malloc(values.count * sizeof(long));

        fseek(inputFile, 0, SEEK_SET);

        while (!feof(inputFile)) {
            for (int i = 0; i < values.count; i++) {
                fscanf(inputFile, "%ld", &values.data[i]);
            }

            switch (type) {
            case NEXT:
                values.value = values.data[values.count - 1];
                break;
            case PREVIOUS:
                values.value = values.data[0];
                break;
            }

            sum += extrapolatedValue(values, type);
        }

        fclose(inputFile);

        free(values.data);
    }

    return sum;
}