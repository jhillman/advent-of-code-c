#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct Command {
    char instruction[4];
    char variable;
    char parameter[4];
};

struct CommandData {
    struct Command *commands;
    int capacity;
    int size;
};

struct CommandData *getCommandData(char *input) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        struct CommandData *data = (struct CommandData *)malloc(sizeof(struct CommandData));
        data->capacity = 50;
        data->size = 0;
        data->commands = (struct Command *)malloc(data->capacity * sizeof(struct Command));

        while (!feof(inputFile)) {
            if (data->size == data->capacity) {
                data->capacity += 50;
                data->commands = (struct Command *)realloc(data->commands, data->capacity * sizeof(struct Command));
            }

            struct Command command;

            fscanf(inputFile, "%s %c", command.instruction, &command.variable);

            if (strcmp(command.instruction, "inp") == 0) {
                strcpy(command.parameter, "");
                data->commands[data->size++] = command;
            } else {
                fscanf(inputFile, "%s", command.parameter);

                data->commands[data->size++] = command;
            }
        }        

        fclose(inputFile);

        return data;
    }

    return NULL;
}

void freeCommandData(struct CommandData *data) {
    free(data->commands);
    free(data);
}

struct IndexOffset {
    int index;
    int offset;
};

struct IndexOffsetStack {
    struct IndexOffset *data;
    int capacity;
    int size;
};

void push(struct IndexOffsetStack *stack, struct IndexOffset value) {
    if (stack->capacity == 0) {
        stack->capacity = 5;
        stack->data = (struct IndexOffset *)malloc(stack->capacity * sizeof(struct IndexOffset));
    } else if (stack->size == stack->capacity) {
        stack->capacity += 5;
        stack->data = (struct IndexOffset *)realloc(stack->data, stack->capacity * sizeof(struct IndexOffset));
    }

    stack->data[stack->size++] = value;
}

struct IndexOffset pop(struct IndexOffsetStack *stack) {
    if (stack->size > 0) {
        return stack->data[--stack->size];
    }

    return (struct IndexOffset){-1, -1};
}

long findModelNumber(struct CommandData *data, bool biggest) {
    struct IndexOffsetStack *stack = (struct IndexOffsetStack *)calloc(1, sizeof(struct IndexOffsetStack));
    int digits[14];
    int targetIndex = 0;

    for (int block = 0; block < data->size; block += 18) {
        int check = atoi(data->commands[block + 5].parameter);
        int offset = atoi(data->commands[block + 15].parameter);

        if (check > 0) {
            push(stack, (struct IndexOffset){targetIndex, offset});
        } else {
            struct IndexOffset indexOffset = pop(stack);
            int totalOffset = indexOffset.offset + check;

            if (totalOffset > 0) {
                if (biggest) {
                    digits[indexOffset.index] = 9 - totalOffset;
                    digits[targetIndex] = 9;
                } else {
                    digits[indexOffset.index] = 1;
                    digits[targetIndex] = 1 + totalOffset;
                }
            } else {
                if (biggest) {
                    digits[indexOffset.index] = 9;
                    digits[targetIndex] = 9 + totalOffset;
                } else {
                    digits[indexOffset.index] = 1 - totalOffset;
                    digits[targetIndex] = 1;
                }
            }
        }

        ++targetIndex;
    }

    free(stack->data);
    free(stack);

    char numberString[15];

    for (int i = 0; i < 14; i++) {
        numberString[i] = '0' + digits[i];
    }

    numberString[14] = '\0';

    return atol(numberString);
}