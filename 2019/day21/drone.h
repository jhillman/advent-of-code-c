#include "../intcode.h"
#include <string.h>

#define HALT -1L

enum Operation {
    AND,
    OR,
    NOT,
    WALK, 
    RUN
};

struct Instruction {
    enum Operation operation;
    char input;
    char output;

    char text[9];
};

struct SpringScript {
    int count;
    struct Instruction* instructions;
};

void addInstruction(struct SpringScript *script, struct Instruction instruction) {
    if (script->count == 0) {
        script->instructions = (struct Instruction *)realloc(script->instructions, 15 * sizeof(struct Instruction));
    }

    script->instructions[script->count++] = instruction;
}

void freeSpringScript(struct SpringScript *script) {
    free(script->instructions);
    free(script);
}

void convertToText(struct SpringScript *script) {
    char *operations[] = { "AND", "OR", "NOT", "WALK", "RUN" };
    struct Instruction *instruction;

    for (int i = 0; i < script->count; i++) {
        instruction = &script->instructions[i];

        if (instruction->operation < WALK) {
            sprintf(instruction->text, "%s %c %c", operations[instruction->operation], instruction->input, instruction->output);
        } else {
            strcpy(instruction->text, operations[instruction->operation]);
        }
    }
}

struct SpringScript *copySpringScript(struct SpringScript *script) {
    struct SpringScript *copy = (struct SpringScript *)calloc(1, sizeof(struct SpringScript));

    for (int i = 0; i < script->count; i++) {
        addInstruction(copy, script->instructions[i]);
    }

    return copy;
}

void addTextInstruction(struct SpringScript *script, char *text) {
    enum Operation operations[] = { AND, OR, NOT, WALK, RUN };
    char indexes[] = "AONWR";
    char *operation;
    char *value;

    struct Instruction instruction;
    strcpy(instruction.text, text);

    operation = strtok(instruction.text, " ");
    instruction.operation = operations[strchr(indexes, *operation) - indexes];

    if ((value = strtok(NULL, " "))) {
        instruction.input = *value;
    } else {
        instruction.input = '-';
    }

    if ((value = strtok(NULL, " "))) {
        instruction.output = *value;
    } else {
        instruction.output = '-';
    }

    strcpy(instruction.text, text);

    addInstruction(script, instruction);
}

void printScript(struct SpringScript *script) {
    for (int i = 0; i < script->count; i++) {
        printf("%s\n", script->instructions[i].text);
    }
}

void freeScript(struct SpringScript *script) {
    free(script->instructions);
    free(script);
}

struct Drone {
    struct SpringScript *script;
    int instruction;
    int offset;
    bool running;
    long damage;
};

long input(void *context) {
    struct Drone *drone = (struct Drone *)context;
    char *instruction = drone->script->instructions[drone->instruction].text;

    if (instruction[drone->offset]) {
        return instruction[drone->offset++];
    } else {
        if (++drone->instruction == drone->script->count) {
            drone->running = true;
        }

        drone->offset = 0;

        return '\n';
    }
}

void output(void *context, long output) {
    struct Drone *drone = (struct Drone *)context;

    if (!drone->running) {
        return;
    }

    if (output > 256) {
        drone->damage = output;
    }
}

long getHullDamage(struct Program *program, struct SpringScript *script, bool showOutput) {
    if (showOutput) {
        long damage = -1;
        char *character;
        long data;

        program->haltValue = HALT;

        pipe(program->input);
        pipe(program->output);

        if (fork() == 0) {
            runProgram(program);

            exit(0);
        }

        while (read(program->output[READ], &data, sizeof(data))) {
            printf("%c", (char)data);
            if (data == '\n') {
                break;
            }
        }

        for (int i = 0; i < script->count; i++) {
            character = script->instructions[i].text;

            while (*character) {
                data = *character;
                printf("%c", (char)data);
                write(program->input[WRITE], &data, sizeof(data));

                ++character;
            }

            data = '\n';
            printf("%c", (char)data);
            write(program->input[WRITE], &data, sizeof(data));
        }

        int newLines = 0;

        while (read(program->output[READ], &data, sizeof(data))) {
            printf("%c", (char)data);

            newLines = newLines + (data == '\n' ? 1 : -newLines);        

            if (newLines == 2) {
                break;
            }
        }

        while (read(program->output[READ], &data, sizeof(data)) && data != HALT) {
            if (data < 256) {
                printf("%c", (char)data);
            } else {
                damage = data;
            }
        }

        return damage;
    } else {
        struct Drone drone = { script, 0, 0, false, -1 };

        if (!program->io) {
            setIO(program, &input, &output, &drone);
        } else {
            resetProgram(program);
            program->io->context = &drone;
        }

        runProgram(program);

        return drone.damage;
    }
}

struct ScriptState {
    char sensors[16];
    struct SpringScript *script;

    struct ScriptState *next;
};

struct ScriptState *copyScriptState(struct ScriptState *state) {
    struct ScriptState *copy = (struct ScriptState *)calloc(1, sizeof(struct ScriptState));
    strcpy(copy->sensors, state->sensors);

    copy->script = copySpringScript(state->script);

    return copy;
}

void freeScriptState(struct ScriptState *state) {
    freeScript(state->script);
    free(state);
}

struct ScriptStateQueue {
    struct ScriptState *head;
    struct ScriptState *tail;
    int size;
};

void enqueue(struct ScriptStateQueue *queue, struct ScriptState *state) {
    if (queue->tail) {
        queue->tail->next = state;
        queue->tail = state;
    } else {
        queue->head = queue->tail = state;
    }

    ++queue->size;
}

struct ScriptState *dequeue(struct ScriptStateQueue *queue) {
    struct ScriptState *state = queue->head;

    if (queue->tail == state) {
        queue->head = queue->tail = NULL;
    } else if (state) {
        queue->head = state->next;
    }

    --queue->size;

    return state;
}

void freeScriptStateQueue(struct ScriptStateQueue *queue) {
    struct ScriptState *state = queue->head;
    struct ScriptState *next;

    while (state) {
        next = state->next;

        freeScriptState(state);
        state = next;
    }

    free(queue);
}

struct Instruction *instructions(char sensor) {
    struct Instruction *sensorInstructions = (struct Instruction *)calloc(9, sizeof(struct Instruction));
    struct Instruction *instruction = sensorInstructions;

    *instruction++ = (struct Instruction) { NOT, sensor, 'J' }; // jump if sensor is not over the hull

    *instruction++ = (struct Instruction) { NOT, sensor, 'T' }; // jump if sensor is over the hull
    *instruction++ = (struct Instruction) { NOT, 'T',    'J' };
        
    *instruction++ = (struct Instruction) { AND, sensor, 'J' }; // jump if J is set and sensor is over the hull
        
    *instruction++ = (struct Instruction) { NOT, sensor, 'T' }; // jump if J is set and sensor is not over the hull
    *instruction++ = (struct Instruction) { AND, 'T',    'J' };

    *instruction++ = (struct Instruction) { OR,  sensor, 'J' }; // jump if J is set or sensor is over the hull

    *instruction++ = (struct Instruction) { NOT, sensor, 'T' }; // jump if J is set or sensor is not over the hull
    *instruction++ = (struct Instruction) { OR , 'T',    'J' };

    return sensorInstructions;
}

struct SpringScript *findScript(struct Program *program, char *sensors, enum Operation operation) {
    struct ScriptStateQueue *queue = (struct ScriptStateQueue *)calloc(1, sizeof(struct ScriptStateQueue));

    struct ScriptState *state = (struct ScriptState *)calloc(1, sizeof(struct ScriptState));
    strcpy(state->sensors, sensors);
    state->script = (struct SpringScript *)calloc(1, sizeof(struct SpringScript));

    struct ScriptState *step;
    struct SpringScript *script = NULL;

    struct Instruction instructions[9] = {
        { NOT, '~', 'J' }, // jump if sensor is not over the hull

        { NOT, '~', 'T' }, // jump if sensor is over the hull
        { NOT, 'T', 'J' },
        
        { AND, '~', 'J' }, // jump if J is set and sensor is over the hull
        
        { NOT, '~', 'T' }, // jump if J is set and sensor is not over the hull
        { AND, 'T', 'J' },

        { OR,  '~', 'J' }, // jump if J is set or sensor is over the hull

        { NOT, '~', 'T' }, // jump if J is set or sensor is not over the hull
        { OR , 'T', 'J' }        
    };

    enqueue(queue, state);

    while (queue->size) {
        state = dequeue(queue);

        convertToText(state->script);

        if (state->script->count > 0 && getHullDamage(program, state->script, false) != -1) {
            script = state->script;
            break;
        }

        char *sensor = state->sensors;
        
        int first;
        int last;

        if (state->script->count == 0) {
            first = 0;
            last = 3;
        } else {
            first = 3;
            last = 9;
        }

        while (*sensor) {
            // struct Instruction *variableInstructions = instructions(*sensor);

            for (int i = first; i < last; i++) {
                struct Instruction instruction = instructions[i];

                if (instruction.input == '~') {
                    instruction.input = *sensor;
                }

                step = copyScriptState(state);
                memcpy(step->sensors, step->sensors + 1, strlen(step->sensors) - 1);
                step->sensors[strlen(step->sensors)] = '\0';

                if (state->script->count > 0) {
                    step->script->count -= 4;
                }

                addInstruction(step->script, instruction);
                addInstruction(step->script, (struct Instruction){ NOT, 'A', 'T' });
                addInstruction(step->script, (struct Instruction){ OR,  'T', 'J' });
                addInstruction(step->script, (struct Instruction){ AND, 'D', 'J' });
                addInstruction(step->script, (struct Instruction){ operation });

                enqueue(queue, step);
            }

            // free(variableInstructions);

            ++sensor;
        }

        freeScriptState(state);
    }

    freeScriptStateQueue(queue);

    return script;
}