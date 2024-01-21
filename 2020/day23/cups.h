#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct Cup {
    int label;
    struct Cup *next;
    struct Cup *previous;
};

struct CupData {
    struct Cup *first;
    struct Cup **cupLookup;
    int cupCount;
};

struct Cup *move(struct Cup *cup, struct Cup **cupLookup, int maxLabel) {
    struct Cup *cup1 = cup->next;
    struct Cup *cup2 = cup1->next;
    struct Cup *cup3 = cup2->next;

    cup->next = cup3->next;
    cup3->previous = cup;

    int destinationLabel = cup->label;
    bool destinationLabelFound = false;

    while (!destinationLabelFound) {
        if (--destinationLabel == 0) {
            destinationLabel = maxLabel;
        }

        destinationLabelFound = destinationLabel != cup1->label &&
            destinationLabel != cup2->label &&
            destinationLabel != cup3->label;
    }

    struct Cup *destinationCup = cupLookup[destinationLabel];
    struct Cup *cupHolder = destinationCup->next;

    destinationCup->next = cup1;
    cup3->next = cupHolder;

    return cup->next;
}

struct CupData *readCupData(char *input) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        struct CupData *data = (struct CupData *) malloc(sizeof(struct CupData));

        char *line = (char *) malloc(10 * sizeof(char));
        size_t length;

        getline(&line, &length, inputFile);

        data->cupLookup = (struct Cup **) malloc((length + 1) * sizeof(struct Cup *));
        char *charPtr = line;

        data->cupCount = strlen(line);

        data->first = (struct Cup *) malloc(sizeof(struct Cup));
        data->first->label = *charPtr - '0';

        struct Cup *cup;
        struct Cup *previous = data->first;
        
        data->cupLookup[data->first->label] = data->first;

        while (*++charPtr) {
            cup = (struct Cup *) malloc(sizeof(struct Cup));
            cup->label = *charPtr - '0';

            data->cupLookup[cup->label] = cup;

            cup->previous = previous;
            previous->next = cup;

            previous = cup;
        }

        cup->next = data->first;
        data->first->previous = cup;

        fclose(inputFile);

        free(line);

        return data;
    }

    return NULL;
}

void freeCupData(struct CupData *data) {
    for (int i = 1; i <= data->cupCount; i++) {
        free(data->cupLookup[i]);
    }

    free(data->cupLookup);
    free(data);
}
