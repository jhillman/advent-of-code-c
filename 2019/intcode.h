#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <stdbool.h>

enum { READ, WRITE };

enum OpCode {
    ADD           = 1,
    MULTIPLY      = 2,
    SAVE          = 3,
    OUTPUT        = 4,
    JUMP_IF_TRUE  = 5,
    JUMP_IF_FALSE = 6,
    LESS_THAN     = 7,
    EQUAL         = 8,
    RELATIVE_BASE = 9,
    HALT_PROGRAM  = 99,
};

enum ParameterMode {
    POSITION  = 0,
    IMMEDIATE = 1,
    RELATIVE  = 2
};

struct ProgramIO {
    void *context;

    long (*input)(void *);
    void (*output)(void *, long);
};

struct Program {
    int identifier;
    
    long *values;
    long *data;
    int length;

    bool running;
    long haltValue;

    int input[2];
    int output[2];

    struct ProgramIO *io;
};

struct Program *loadProgram(char *filename) {
    FILE *programFile = fopen(filename, "r");

    if (programFile) {
        char ch;
        int length = 0;

        while (!feof(programFile)) {
            ch = fgetc(programFile);

            if (ch == ',') {
                ++length;
            }
        }

        if (ch != ',' && length > 0) {
            ++length;
        }

        struct Program *program = (struct Program *)calloc(1, sizeof(struct Program));

        program->values = (long *)calloc(length * 2, sizeof(long));
        program->data = (long *)calloc(length * 2, sizeof(long));
        program->length = length;

        int index = 0;

        fseek(programFile, 0, SEEK_SET);

        long input;

        while (index < length) {
            fscanf(programFile, "%ld%c", &input, &ch);

            program->values[index] = input;
            program->data[index] = input;

            ++index;
        }

        fclose(programFile);

        return program;
    }

    return NULL;
}

void setIO(struct Program *program, long (*input)(void *), void (*output)(void *, long), void *context) {
    if (program->io) {
        free(program->io);
    }

    program->io = (struct ProgramIO *)calloc(1, sizeof(struct ProgramIO));
    program->io->input = input;
    program->io->output = output;
    program->io->context = context;
}

struct Program *copyProgram(struct Program *program, int identifier) {
    struct Program *programCopy = (struct Program *)calloc(1, sizeof(struct Program));

    programCopy->identifier = identifier;
    programCopy->values = (long *)calloc(program->length, sizeof(long));
    programCopy->data = (long *)calloc(program->length, sizeof(long));

    for (int i = 0; i < program->length; i++) {
        programCopy->values[i] = program->values[i];
        programCopy->data[i] = program->data[i];
    }

    programCopy->length = program->length;

    return programCopy;
}

void resetProgram(struct Program *program) {
    for (int i = 0; i < program->length; i++) {
        program->data[i] = program->values[i];
    }
}

void initializeProgram(struct Program *program, int noun, int verb) {
    program->data[1] = noun;
    program->data[2] = verb;
}

int programNoun(struct Program *program) {
    return program->data[1];
}

int programVerb(struct Program *program) {
    return program->data[2];
}

void printProgram(struct Program *program) {
    for (int i = 0; i < program->length; i++) {
        printf("%ld%s", program->data[i], i < program->length - 1 ? "," : "");
    }

    printf("\n");
}

long readParameter(struct Program *program, enum ParameterMode mode, int offset, int relativeBase) {
    switch (mode) {
        case POSITION:
            return program->data[program->data[offset]];
        case IMMEDIATE:
            return program->data[offset];
        case RELATIVE:
            return program->data[relativeBase + program->data[offset]];
    }
}

void writeData(struct Program *program, enum ParameterMode mode, int offset, int relativeBase, long data) {
    switch (mode) {
        case POSITION:
            program->data[program->data[offset]] = data;
            break;
        case IMMEDIATE:
            program->data[offset] = data;
            break;
        case RELATIVE:
            program->data[relativeBase + program->data[offset]] = data;
            break;
    }
}

void runProgram(struct Program *program) {
    int offset = 0;
    int relativeBase = 0;

    program->running = true;

    while (program->running && offset < program->length) {
        int instruction = program->data[offset];

        enum OpCode opcode = instruction % 100;
        instruction = instruction / 100;

        enum ParameterMode mode1 = instruction % 10;
        instruction /= 10;

        enum ParameterMode mode2 = instruction % 10;
        instruction /= 10;

        enum ParameterMode mode3 = instruction;

        long parameter1;
        long parameter2;

        switch (opcode) {
            case ADD:
                parameter1 = readParameter(program, mode1, offset + 1, relativeBase);
                parameter2 = readParameter(program, mode2, offset + 2, relativeBase);

                writeData(program, mode3, offset + 3, relativeBase, parameter1 + parameter2);

                offset += 4;
                break;
            case MULTIPLY:
                parameter1 = readParameter(program, mode1, offset + 1, relativeBase);
                parameter2 = readParameter(program, mode2, offset + 2, relativeBase);

                writeData(program, mode3, offset + 3, relativeBase, parameter1 * parameter2);
        
                offset += 4;
                break;
            case SAVE:
                if (program->input[READ]) {
                    while (!read(program->input[READ], &parameter1, sizeof(parameter1)));
                } else if (program->io && program->io->input) {
                    parameter1 = program->io->input(program->io->context);
                } else {
                    scanf("%ld", &parameter1);
                }

                writeData(program, mode1, offset + 1, relativeBase, parameter1);

                offset += 2;
                break;
            case OUTPUT:
                parameter1 = readParameter(program, mode1, offset + 1, relativeBase);

                if (program->output[WRITE]) {
                    write(program->output[WRITE], &parameter1, sizeof(parameter1));
                } else if (program->io && program->io->output) {
                    program->io->output(program->io->context, parameter1);
                } else {
                    printf("%ld\n", parameter1);
                }

                offset += 2;
                break;
            case JUMP_IF_TRUE:
                parameter1 = readParameter(program, mode1, offset + 1, relativeBase);
                parameter2 = readParameter(program, mode2, offset + 2, relativeBase);

                if (parameter1) {
                    offset = parameter2;
                } else {
                    offset += 3;
                }
                break;
            case JUMP_IF_FALSE:
                parameter1 = readParameter(program, mode1, offset + 1, relativeBase);
                parameter2 = readParameter(program, mode2, offset + 2, relativeBase);

                if (!parameter1) {
                    offset = parameter2;
                } else {
                    offset += 3;
                }
                break;
            case LESS_THAN:
                parameter1 = readParameter(program, mode1, offset + 1, relativeBase);
                parameter2 = readParameter(program, mode2, offset + 2, relativeBase);

                writeData(program, mode3, offset + 3, relativeBase, parameter1 < parameter2 ? 1 : 0);

                offset += 4;
                break;
            case EQUAL:
                parameter1 = readParameter(program, mode1, offset + 1, relativeBase);
                parameter2 = readParameter(program, mode2, offset + 2, relativeBase);

                writeData(program, mode3, offset + 3, relativeBase, parameter1 == parameter2 ? 1 : 0);

                offset += 4;
                break;
            case RELATIVE_BASE:
                parameter1 = readParameter(program, mode1, offset + 1, relativeBase);
                
                relativeBase += parameter1;

                offset += 2;
                break;    
            case HALT_PROGRAM:
                program->running = false;

                if (program->haltValue) {
                    if (program->output[WRITE]) {
                        write(program->output[WRITE], &program->haltValue, sizeof(program->haltValue));
                    } else {
                        printf("%ld\n", program->haltValue);                        
                    }
                }
                break;
        }
    }
}

void freeProgram(struct Program *program) {
    if (program) {
        free(program->values);
        free(program->data);

        if (program->io) {
            free(program->io);
        }

        if (program->input[READ]) {
            close(program->input[READ]);
        }
        if (program->input[WRITE]) {
            close(program->input[WRITE]);
        }

        if (program->output[READ]) {
            close(program->output[READ]);
        }
        if (program->output[WRITE]) {
            close(program->output[WRITE]);
        }

        free(program);
    }
}