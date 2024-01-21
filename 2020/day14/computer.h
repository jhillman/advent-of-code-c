#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MASK_LENGTH 36

enum InstructionType {
    MASK,
    MEMORY
};

struct Instruction {
    enum InstructionType type;
    char mask[MASK_LENGTH + 1];
    int memoryOffset;
    long value;
};

struct Computer {
    struct Instruction *instructions;
    int instructionCount;
    long *memory;
};

struct Computer *getComputer(char *input) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        struct Computer *computer = (struct Computer *) calloc(1, sizeof(struct Computer *));

        computer->instructionCount = 0;
        char ch;

        while (!feof(inputFile)) {
            ch = fgetc(inputFile);

            if (feof(inputFile) || ch == '\n') {
                ++computer->instructionCount;
            }
        }

        fseek(inputFile, 0, SEEK_SET);

        char line[50];
        computer->instructions = (struct Instruction *) calloc(computer->instructionCount, sizeof(struct Instruction));
        int instructionIndex = 0;

        while (fgets(line, sizeof(line), inputFile)) {
            if (strstr(line, "mask")) {
                computer->instructions[instructionIndex].type = MASK;

                sscanf(line, "mask = %s", computer->instructions[instructionIndex].mask);
            } else if (strstr(line, "mem")) {
                computer->instructions[instructionIndex].type = MEMORY;

                sscanf(line, "mem[%d] = %ld", &computer->instructions[instructionIndex].memoryOffset, &computer->instructions[instructionIndex].value);
            }

            ++instructionIndex;
        }

        fclose(inputFile);

        computer->memory = (long *)calloc(1LL << MASK_LENGTH, sizeof(long));

        return computer;
    }

    return NULL;
}

void freeComputer(struct Computer *computer) {
    free(computer->instructions);
    free(computer->memory);
    free(computer);
}