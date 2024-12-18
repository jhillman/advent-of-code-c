#include <stdlib.h>
#include <stdbool.h>

enum Opcode {
    ADV,
    BXL,
    BST,
    JNZ,
    BXC,
    OUT,
    BDV,
    CDV    
};

struct Computer {
    long a;
    long b;
    long c;
    long *program;
    int programLength;
    int instruction;
};

struct Output {
    char *string;
    long *integer;
    bool (* evaluate)(long);
};

long getOperand(struct Computer *computer, long operand) {
    switch (operand) {
    case 4:
        return computer->a;
    case 5:
        return computer->b;
    case 6:
        return computer->c;
    default:
        return operand;
    }
}

struct Computer *getComputer(char *input) {
    FILE *inputFile = fopen(input, "r");
    struct Computer *computer = NULL;

    if (inputFile) {
        computer = (struct Computer *)calloc(1, sizeof(struct Computer));
        char program[32];
        char *instruction;

        if (fscanf(inputFile, "Register A: %ld\nRegister B: %ld\nRegister C: %ld\n\nProgram: %s", 
            &computer->a, &computer->b, &computer->c, program)) {
            instruction = program;

            computer->program = (long *)malloc(strlen(program) - (strlen(program) / 2) - 1 * sizeof(long));
            computer->programLength = 0;

            while (*instruction) {
                computer->program[computer->programLength++] = *instruction++ - '0';
                computer->program[computer->programLength++] = *++instruction - '0';

                if (*++instruction) {
                    ++instruction;
                }
            }
        }

        fclose(inputFile);
    }

    return computer;
}

void freeComputer(struct Computer *computer) {
    free(computer->program);
    free(computer);
}

void run(struct Computer *computer, struct Output *output) {
    long opcode;
    long operand;
    long out;
    char comma[] = ",";
    char nothing[] = "";
    char *separator;
    int characterCount = 0;

    while (computer->instruction < computer->programLength) {
        opcode = computer->program[computer->instruction++];
        operand = computer->program[computer->instruction++];

        switch (opcode) {
        case ADV:
            computer->a >>= getOperand(computer, operand);
            break;
        case BXL:
            computer->b ^= operand;
            break;
        case BST:
            computer->b = getOperand(computer, operand) % 8;
            break;
        case JNZ:
            computer->instruction = computer->a ? operand : computer->instruction;
            break;
        case BXC:
            computer->b ^= computer->c;
            break;
        case OUT:
            separator = characterCount > 0 ? comma : nothing;
            out = getOperand(computer, operand) % 8;

            if (output && output->string) {
                characterCount = sprintf(output->string, "%s%ld", separator, out);
                output->string += characterCount;
            } else if (output && output->integer) {
                *output->integer *= 10;
                *output->integer += out;
            } else {
                characterCount = printf("%s%ld", separator, out);
            }

            if (output->evaluate && output->evaluate(out)) {
                computer->instruction = computer->programLength;
            }
            break;
        case BDV:
            computer->b = computer->a >> getOperand(computer, operand);
            break;
        case CDV:
            computer->c = computer->a >> getOperand(computer, operand);
            break;
        }
    }
}