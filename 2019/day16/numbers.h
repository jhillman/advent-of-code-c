#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct NumberList {
    int length;
    int *numbers;
};

void addNumber(struct NumberList *list, int number) {
    list->numbers = (int *)realloc(list->numbers, (list->length + 1) * sizeof(int));
    list->numbers[list->length++] = number;
}

struct NumberList *copyList(struct NumberList *list) {
    struct NumberList *copy = (struct NumberList *)calloc(1, sizeof(struct NumberList));
    copy->length = list->length;
    copy->numbers = (int *)malloc(copy->length * sizeof(int));

    memcpy(copy->numbers, list->numbers, list->length * sizeof(int));

    return copy;
}

struct NumberList *getNumberList(char *input) {
    FILE *inputFile = fopen(input, "r");
    struct NumberList *list = NULL;

    if (inputFile) {
        list = (struct NumberList *)calloc(1, sizeof(struct NumberList));

        char c = fgetc(inputFile);;

        do {
            addNumber(list, c - '0');

            c = fgetc(inputFile);
        } while (c && !feof(inputFile));

        fclose(inputFile);
    }

    return list;
}

void freeNumberList(struct NumberList *list) {
    free(list->numbers);
    free(list);
}