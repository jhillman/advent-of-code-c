#include <stdlib.h>
#include <string.h>

enum Opcode {
    ADDR,
    ADDI,
    MULR,
    MULI,
    BANR,
    BANI,
    BORR,
    BORI,
    SETR,
    SETI,
    GTIR,
    GTRI,
    GTRR,
    EQIR,
    EQRI,
    EQRR,
    DIVI,
    NOOP
};

enum Opcode parseOpcode(char *code) {
    enum Opcode opcode;

    if (strcmp(code, "addr") == 0) {
        return ADDR;
    } else if (strcmp(code, "addi") == 0) {
        return ADDI;
    } else if (strcmp(code, "mulr") == 0) {
        return MULR;
    } else if (strcmp(code, "muli") == 0) {
        return MULI;
    } else if (strcmp(code, "banr") == 0) {
        return BANR;
    } else if (strcmp(code, "bani") == 0) {
        return BANI;
    } else if (strcmp(code, "borr") == 0) {
        return BORR;
    } else if (strcmp(code, "bori") == 0) {
        return BORI;
    } else if (strcmp(code, "setr") == 0) {
        return SETR;
    } else if (strcmp(code, "seti") == 0) {
        return SETI;
    } else if (strcmp(code, "gtir") == 0) {
        return GTIR;
    } else if (strcmp(code, "gtri") == 0) {
        return GTRI;
    } else if (strcmp(code, "gtrr") == 0) {
        return GTRR;
    } else if (strcmp(code, "eqir") == 0) {
        return EQIR;
    } else if (strcmp(code, "eqri") == 0) {
        return EQRI;
    } else if (strcmp(code, "eqrr") == 0) {
        return EQRR;
    } else if (strcmp(code, "divi") == 0) {
        return DIVI;
    }

    return NOOP;
}

struct Instruction {
    enum Opcode opcode;
    int a;
    int b;
    int c;
};

struct Program {
    int length;
    struct Instruction *instructions;
    int registers[6];
    int instructionPointer;
    int *instructionPointerRegister;
};

void addInstruction(struct Program *program, struct Instruction instruction) {
    program->instructions = (struct Instruction *)realloc(program->instructions, (program->length + 1) * sizeof(struct Instruction));
    program->instructions[program->length++] = instruction;
}

struct Program *readProgram(char *input) {
    FILE *inputFile = fopen(input, "r");
    struct Program *program = NULL;

    if (inputFile) {
        int instructionPointerRegister;
        char opcode[5];
        int a;
        int b;
        int c;

        program = (struct Program *)calloc(1, sizeof(struct Program));

        fscanf(inputFile, "#ip %d\n", &instructionPointerRegister);

        program->instructionPointerRegister = &program->registers[instructionPointerRegister];

        while (!feof(inputFile) && fscanf(inputFile, "%[^ ] %d %d %d\n", opcode, &a, &b, &c) == 4) {
            addInstruction(program, (struct Instruction){ parseOpcode(opcode), a, b, c });
        }
    }

    return program;
}

void operation(int *registers, enum Opcode code, int a, int b, int c) {
    switch (code) {
    case ADDR:
        registers[c] = registers[a] + registers[b];
        break;
    case ADDI:
        registers[c] = registers[a] + b;
        break;
    case MULR:
        registers[c] = registers[a] * registers[b];
        break;
    case MULI:
        registers[c] = registers[a] * b;
        break;
    case BANR:
        registers[c] = registers[a] & registers[b];
        break;
    case BANI:
        registers[c] = registers[a] & b;
        break;
    case BORR:
        registers[c] = registers[a] | registers[b];
        break;
    case BORI:
        registers[c] = registers[a] | b;
        break;
    case SETR:
        registers[c] = registers[a];
        break;
    case SETI:
        registers[c] = a;
        break;
    case GTIR:
        registers[c] = a > registers[b] ? 1 : 0;
        break;
    case GTRI:
        registers[c] = registers[a] > b ? 1 : 0;
        break;
    case GTRR:
        registers[c] = registers[a] > registers[b] ? 1 : 0;
        break;
    case EQIR:
        registers[c] = a == registers[b] ? 1 : 0;
        break;
    case EQRI:
        registers[c] = registers[a] == b ? 1 : 0;
        break;
    case EQRR:
        registers[c] = registers[a] == registers[b] ? 1 : 0;
        break;
    case DIVI:
        registers[c] = registers[a] / b;
        break;
    case NOOP:
        break;
    }
}

void execute(struct Program *program) {
    struct Instruction instruction = program->instructions[program->instructionPointer];

    *program->instructionPointerRegister = program->instructionPointer;
    operation(program->registers, instruction.opcode, instruction.a, instruction.b, instruction.c);

    program->instructionPointer = *program->instructionPointerRegister;

    ++program->instructionPointer;
}

void runProgram(struct Program *program) {
    while (program->instructionPointer < program->length) {
        execute(program);
    }
}

void freeProgram(struct Program *program) {
    free(program->instructions);
    free(program);
}
