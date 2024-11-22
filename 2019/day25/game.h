#include "../../aoc.h"
#include "../intcode.h"
#include <sys/ioctl.h>
#include <signal.h>
#include <time.h>

#define SHOW_OUTPUT 0

enum GameMode {
    MANUAL,
    AUTOMATIC,
    INSTRUCTIONS
};

void msleep(long msec) {
    struct timespec ts;

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    while(nanosleep(&ts, &ts));
}

struct StringList {
    int capacity;
    int count;
    char **list;
    int index;
};

int maxStringsCapacity = 0;

void addString(struct StringList *strings, char *string) {
    if (strings->count == strings->capacity) {
        strings->capacity += 10;

        if (strings->capacity > maxStringsCapacity) {
            maxStringsCapacity = strings->capacity;
        }

        strings->list = (char **)realloc(strings->list, strings->capacity * sizeof(char *));
    }

    strings->list[strings->count] = (char *)malloc((strlen(string) + 1) * sizeof(char));
    strcpy(strings->list[strings->count++], string);
}

void removeString(struct StringList *strings, char *string) {
    bool removed = false;

    for (int i = 0; i < strings->count; i++) {
        if (removed) {
            strings->list[i - 1] = strings->list[i];
        } else if (strcmp(strings->list[i], string) == 0) {
            free(strings->list[i]);

            removed = true;
        }
    }

    if (removed) {
        --strings->count;
    }
}

struct StringList *copyStringList(struct StringList *strings) {
    struct StringList *copy = (struct StringList *)calloc(1, sizeof(struct StringList));

    for (int i = 0; i < strings->count; i++) {
        addString(copy, strings->list[i]);
    }

    return copy;
}

void clearStringList(struct StringList *strings) {
    for (int i = 0; i < strings->count; i++) {
        if (strings->list[i]) {
            free(strings->list[i]);
        }
    }

    strings->count = 0;
}

void freeStringList(struct StringList *strings) {
    clearStringList(strings);

    free(strings);
}

struct Identity {
    int id;
    char name[32];
};

struct IdentityStore {
    int count;
    struct Identity *identities;
};

int compareIdentities(const void *a, const void *b) {
    return strcmp(((struct Identity *)a)->name, ((struct Identity *)b)->name);
}

int findId(struct IdentityStore *store, char *name, int low, int high) {
    if (store->count == 0 || low > high) {
        return -1;
    }

    int middle = low + ((high - low) / 2);

    if (middle < store->count) {
        int result = strcmp(name, store->identities[middle].name);

        if (result == 0) {
            return store->identities[middle].id;
        } else if (result < 0) {
            return findId(store, name, low, middle - 1);
        } else {
            return findId(store, name, middle + 1, high);
        }    
    }

    return -1;
}

int getId(struct IdentityStore *store, char *name) {
    int id = findId(store, name, 0, store->count);

    if (id == -1) {
        id = store->count;

        struct Identity identity;
        identity.id = id;
        strcpy(identity.name, name);

        store->identities = (struct Identity *)realloc(store->identities, (store->count + 1) * sizeof(struct Identity));

        store->identities[store->count++] = identity;

        qsort(store->identities, store->count, sizeof(struct Identity), compareIdentities);
    }

    return id;
}

void freeIdentidyStore(struct IdentityStore *store) {
    if (store) {
        free(store->identities);

        free(store);
    }
}

struct InstructionState {
    long inventory;
    long inventoryRoomInstruction;
    struct StringList *instructions;

    struct InstructionState *next;
};

void freeInstructionState(struct InstructionState *state) {
    freeStringList(state->instructions);

    free(state);
}

struct InstructionStateList {
    int capacity;
    int count;
    struct InstructionState **list;
};

int compareInstructionStates(const void *a, const void *b) {
    long inventoryRoomInstructionA = (*((struct InstructionState **)a))->inventoryRoomInstruction;
    long inventoryRoomInstructionB = (*((struct InstructionState **)b))->inventoryRoomInstruction;

    if (inventoryRoomInstructionA < inventoryRoomInstructionB) {
        return -1;
    } else if (inventoryRoomInstructionA > inventoryRoomInstructionB) {
        return 1;
    } else {
        return 0;
    }
}

struct InstructionState *findInstructionState(struct InstructionStateList *states, long inventoryRoomInstruction, int low, int high) {
    if (states == NULL || states->list == NULL || states->count == 0 || low > high) {
        return NULL;
    }

    int middle = low + ((high - low) / 2);

    if (middle < states->count) {
        long result = inventoryRoomInstruction - states->list[middle]->inventoryRoomInstruction;

        if (result == 0) {
            return states->list[middle];
        } else if (result < 0) {
            return findInstructionState(states, inventoryRoomInstruction, low, middle - 1);
        } else {
            return findInstructionState(states, inventoryRoomInstruction, middle + 1, high);
        }
    }

    return NULL;
}

void addInstructionState(struct InstructionStateList *states, struct InstructionState *state) {
    if (states->count == states->capacity) {
        states->capacity += 1000;
        states->list = (struct InstructionState **)realloc(states->list, states->capacity * sizeof(struct InstructionState *));
    }

    states->list[states->count++] = state;

    qsort(states->list, states->count, sizeof(struct InstructionState *), compareInstructionStates);
}

void freeInstructionStateList(struct InstructionStateList* states) {
    if (states->list) {
        free(states->list);
    }

    free(states);
}

struct InstructionStateQueue {
    int size;
    struct InstructionState *head;
    struct InstructionState *tail;

    struct InstructionStateList *states;
};


void enqueue(struct InstructionStateQueue *queue, long inventory, long inventoryRoomInstruction, struct StringList *instructions, char *instruction) {
    struct InstructionState *state = (struct InstructionState *)calloc(1, sizeof(struct InstructionState));
    state->inventory = inventory;
    state->inventoryRoomInstruction = inventoryRoomInstruction;

    if (instructions) {
        state->instructions = copyStringList(instructions);
    } else {
        state->instructions = (struct StringList *)calloc(1, sizeof(struct StringList));
    }

    if (instruction) {
        addString(state->instructions, instruction);
    }

    if (queue->tail) {
        queue->tail->next = state;
        queue->tail = state;
    } else {
        queue->head = queue->tail = state;
    }

    ++queue->size;

    addInstructionState(queue->states, state);
}

struct InstructionState *dequeue(struct InstructionStateQueue *queue) {
    struct InstructionState *state = queue->head;

    if (queue->tail == state) {
        queue->head = queue->tail = NULL;
    } else if (state) {
        queue->head = state->next;
    }

    --queue->size;

    return state;
}

void freeInstructionStateQueue(struct InstructionStateQueue *queue) {
    if (queue) {
        freeInstructionStateList(queue->states);

        free(queue);
    }
}

struct Room {
    char name[32];
    struct StringList *doors;
    struct StringList *items;
};

void freeRoom(struct Room *room) {
    if (room) {
        freeStringList(room->doors);
        freeStringList(room->items);

        free(room);
    }
}

struct Droid {
    struct Program *program;
    char *input;
    char *output;

    struct StringList *observations;
    char observation[256];
    char lastObservation[256];

    struct IdentityStore *store;

    struct Room *room;
    struct InstructionState *state;
    struct InstructionStateQueue *queue;

    int code;
};

bool oppositeInstruction(char *instruction, char *previousInstruction) {
    switch (*instruction) {
    case 'n':
        return *previousInstruction == 's';
    case 's':
        return *previousInstruction == 'n';
    case 'e':
        return *previousInstruction == 'w';
    case 'w':
        return *previousInstruction == 'e';
    default:
        return false;
    }
}

void evaluateInstruction(struct Droid *droid, char *instruction) {
    long inventory = 0;
    long inventoryRoomInstruction = 0;
    bool opposite = false;

    if (droid->state) {
        inventory = droid->state->inventory;
        inventoryRoomInstruction = droid->state->inventory;

        if (droid->state->instructions->count > 0) {
            opposite = oppositeInstruction(instruction, droid->state->instructions->list[droid->state->instructions->count - 1]);
        }
    }

    inventoryRoomInstruction |= (1L << getId(droid->store, droid->room->name));
    inventoryRoomInstruction |= (1L << getId(droid->store, instruction));

    struct InstructionState *state;

    if (!opposite && ((state = findInstructionState(droid->queue->states, inventoryRoomInstruction, 0, droid->queue->states->count)) == NULL || state->instructions->count > droid->state->instructions->count + 1)) {
        enqueue(droid->queue, inventory, inventoryRoomInstruction, droid->state ? droid->state->instructions : NULL, instruction);
    }
}

void freeDroid(struct Droid *droid) {
    freeStringList(droid->observations);
    freeIdentidyStore(droid->store);

    freeRoom(droid->room);
    freeInstructionStateQueue(droid->queue);

    free(droid);
}

long input(void *context) {
    struct Droid *droid = (struct Droid *)context;

    if (droid->input && droid->state) {
        if (*droid->input) {
            if (SHOW_OUTPUT) {
                fputc(*droid->input, stdout);
                fflush(stdout);
            }

            return *droid->input++;
        } else {
            if (SHOW_OUTPUT) {
                fputc('\n', stdout);
                fflush(stdout);
            }

            if (droid->state->instructions->index < droid->state->instructions->count) {
                droid->input = droid->state->instructions->list[droid->state->instructions->index++];
            } else {
                droid->input = NULL;
            }

            return '\n';
        }
    } else {
        droid->program->running = false;
    }

    return 0;
}

void output(void *context, long output) {
    struct Droid *droid = (struct Droid *)context;
    char character = (char)output;

    if (SHOW_OUTPUT) {
        fputc(character, stdout);
        fflush(stdout);
    }

    if (character == '\n') {
        *droid->output = '\0';

        if (strlen(droid->observation) > 0) {
            if (strcmp(droid->observation, droid->lastObservation) == 0) {
                droid->program->running = false;
                strcpy(droid->lastObservation, "");

                return;
            }

            strcpy(droid->lastObservation, droid->observation);

            addString(droid->observations, droid->observation);
        }

        droid->output = droid->observation;
    } else {
        *droid->output++ = character;
    }
}

int playTextAdventure(struct Program *program) {
    int data;
    char response[33];
    char *character;

    printf("Enter 'quit' to stop.\n");

    pipe(program->input);
    pipe(program->output);

    program->identifier = fork();

    if (program->identifier == 0) {
        close(program->input[WRITE]);
        close(program->output[READ]);

        runProgram(program);

        exit(0);
    }

    data = 1;
    ioctl(program->input[WRITE], FIONBIO, &data);
    ioctl(program->output[READ], FIONBIO, &data);

    close(program->input[READ]);
    close(program->output[WRITE]);

    bool questionPrinted = false;
    bool getCommand = false;

    while (true) {
        int bytesRead = read(program->output[READ], &data, sizeof(data));

        while (bytesRead > 0) {
            msleep(5);
            fputc((char)data, stdout);
            fflush(stdout);

            bytesRead = read(program->output[READ], &data, sizeof(data));

            if (bytesRead <= 0) {
                getCommand = true;
                break;
            }
        }

        msleep(200);

        if (getCommand) {
            fflush(stdin);
            scanf("%32[^\n]", response);

            if (strstr(response, "quit")) {
                break;
            }

            character = response;

            while (*character) {
                data = *character;
                write(program->input[WRITE], &data, sizeof(data));

                ++character;
                msleep(10);
            }

            data = '\n';
            write(program->input[WRITE], &data, sizeof(data));

            msleep(10);
            getCommand = false;
        }
    }

    return 0;
}

void checkForCode(struct Droid *droid, char *observation) {
    char *code = strstr(droid->observation, "get in by typing");

    if (code) {
        sscanf(code + 16, "%d", &droid->code);
    }
}

void checkObservations(struct Droid *droid) {
    int observationIndex = droid->observations->count - 1;
    char *observation = droid->observations->list[observationIndex];
    char item[32];

    if (observation && strcmp(observation, "Command?") == 0) {
        do {
            --observationIndex;
            observation = droid->observations->list[observationIndex];
        } while (*observation != '=');

        strcpy(droid->room->name, observation);

        clearStringList(droid->room->doors);
        clearStringList(droid->room->items);

        while (observationIndex < droid->observations->count) {
            observation = droid->observations->list[observationIndex];

            if (strcmp(observation, "Doors here lead:") == 0) {
                observation = droid->observations->list[observationIndex + 1];

                while (*observation == '-') {
                    addString(droid->room->doors, observation + 2);
                    
                    observation = droid->observations->list[++observationIndex + 1];
                }
            } else if (strcmp(observation, "Items here:") == 0) {
                observation = droid->observations->list[observationIndex + 1];

                while (*observation == '-') {
                    addString(droid->room->items, observation + 2);
                    
                    observation = droid->observations->list[++observationIndex + 1];
                }
            } if (strstr(observation, "You take the")) {
                strcpy(item, observation + 13);
                item[strcspn(item, ".")] = '\0';

                droid->state->inventory |= (1L << getId(droid->store, item));

                removeString(droid->room->items, item);
            }

            ++observationIndex;
        }

        for (int i = 0; i < droid->room->doors->count; i++) {
            evaluateInstruction(droid, droid->room->doors->list[i]);
        }

        for (int i = 0; i < droid->room->items->count; i++) {
            sprintf(item, "take %s", droid->room->items->list[i]);

            evaluateInstruction(droid, item);
        }
    } else {
        checkForCode(droid, observation);
    }
}

int playAutomatically(struct Program *program) {
    struct Droid *droid = (struct Droid *)calloc(1, sizeof(struct Droid));
    int code = 0;

    droid->program = program;
    setIO(droid->program, &input, &output, droid);

    droid->observations = (struct StringList *)calloc(1, sizeof(struct StringList));
    
    droid->store = (struct IdentityStore *)calloc(1, sizeof(struct IdentityStore));

    droid->room = (struct Room *)calloc(1, sizeof(struct Room));
    droid->room->doors = (struct StringList *)calloc(1, sizeof(struct StringList));
    droid->room->items = (struct StringList *)calloc(1, sizeof(struct StringList));

    droid->queue = (struct InstructionStateQueue *)(calloc(1, sizeof(struct InstructionStateQueue)));
    droid->queue->states = (struct InstructionStateList *)(calloc(1, sizeof(struct InstructionStateList)));

    do {
        clearStringList(droid->observations);
        
        droid->output = droid->observation;

        runProgram(droid->program);
        resetProgram(droid->program);

        checkObservations(droid);

        if (droid->queue->size > 0) {
            droid->state = dequeue(droid->queue);
            droid->input = droid->state->instructions->list[droid->state->instructions->index++];

        } else {
            break;
        }
    } while (!droid->code);

    code = droid->code;

    freeDroid(droid);

    return code;
}

int playInstructions(struct Program *program) {
    struct Droid *droid = (struct Droid *)calloc(1, sizeof(struct Droid));
    int code = 0;

    char *instructions[] = {
        "north",
        "east",
        "north",
        "take loom",
        "south",
        "west",
        "north",
        "north",
        "take mutex",
        "south",
        "south",
        "south",
        "west",
        "west",
        "south",
        "east",
        "north",
        "take wreath",
        "south",
        "south",
        "west",
        "north",
        "take sand",
        "north",
        "east",
        "east"
    };

    droid->program = program;
    setIO(droid->program, &input, &output, droid);

    droid->observations = (struct StringList *)calloc(1, sizeof(struct StringList));
    droid->state = (struct InstructionState *)calloc(1, sizeof(struct InstructionState));
    droid->state->instructions = (struct StringList *)calloc(1, sizeof(struct StringList));

    for (int i = 0; i < sizeof(instructions) / sizeof(char *); i++) {
        addString(droid->state->instructions, instructions[i]);
    }

    droid->output = droid->observation;
    droid->input = droid->state->instructions->list[droid->state->instructions->index++];

    runProgram(droid->program);
    checkForCode(droid, droid->observation);

    code = droid->code;

    free(droid->state);
    freeDroid(droid);

    return code;
}

int playGame(struct Program *program, enum GameMode mode) {
    switch (mode) {
    case MANUAL:
        return playTextAdventure(program);
    case AUTOMATIC:
        return playAutomatically(program);
    case INSTRUCTIONS:
        return playInstructions(program);
    }
}