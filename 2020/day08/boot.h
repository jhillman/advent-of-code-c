#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

enum Operation {
    NOP,
    ACC,
    JMP
};

struct Instruction {
    enum Operation operation;
    int argument;
    int runCount;
    bool changed;
};

struct BootData {
    struct Instruction *instructions;
    int instructionCount;
};

struct BootData readInput(char *input) {
    FILE *inputFile = fopen(input, "r");

	struct BootData data;
	data.instructionCount = 0;

    if (inputFile) {
        char ch;

        while (!feof(inputFile)) {
            ch = fgetc(inputFile);

            if (ch == '\n') {
                ++data.instructionCount;
            }
        }

        if (ch != '\n' && data.instructionCount > 0) {
            ++data.instructionCount;
        }

        fseek(inputFile, 0, SEEK_SET);

        data.instructions = (struct Instruction *)calloc(data.instructionCount, sizeof(struct Instruction));

        char operation[4];
        int argument;
        int instructionIndex = 0;

        while (instructionIndex < data.instructionCount) {
            fscanf(inputFile, "%s %d", operation, &argument);

            if (strcmp(operation, "acc") == 0) {
                data.instructions[instructionIndex].operation = ACC;
            } else if (strcmp(operation, "jmp") == 0) {
                data.instructions[instructionIndex].operation = JMP;
            } else {
                data.instructions[instructionIndex].operation = NOP;
            }

            data.instructions[instructionIndex].argument = argument;
            data.instructions[instructionIndex].runCount = 0;
            data.instructions[instructionIndex].changed = false;

            ++instructionIndex;
        }

        fclose(inputFile);
    }

    return data;
}

void freeData(struct BootData data) {
	free(data.instructions);
}