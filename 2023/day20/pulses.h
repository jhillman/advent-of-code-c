#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

enum ModuleType {
    UNKNOWN,
    BUTTON,
    BROADCAST,
    FLIP_FLOP,
    CONJUNCTION
};

enum State {
    OFF,
    ON
};

enum Pulse {
    LOW,
    HIGH
};

struct Pulses {
    int capacity;
    int count;
    enum Pulse *data;
};

void addPulse(struct Pulses *pulses, enum Pulse pulse) {
    if (pulses->count == pulses->capacity) {
        pulses->capacity += 10;
        pulses->data = (enum Pulse *)realloc(pulses->data, pulses->capacity * sizeof(enum Pulse));
    }

    pulses->data[pulses->count++] = pulse;
}

struct Modules;

struct Module {
    char name[16];
    enum ModuleType type;
    enum State state;
    enum Pulse pulse;

    struct Modules *inputs;
    struct Modules *destinations;
};

struct Modules {
    int capacity;
    int count;
    struct Module **data;
};

void addModule(struct Modules *modules, struct Module *module) {
    if (modules->count == modules->capacity) {
        modules->capacity += 10;
        modules->data = (struct Module **)realloc(modules->data, modules->capacity * sizeof(struct Module *));
    }

    modules->data[modules->count++] = module;
}

struct Module *getModule(struct Modules *modules, char name[]) {
    struct Module *module = NULL;

    for (int i = 0; !module && i < modules->count; i++) {
        if (strcmp(modules->data[i]->name, name) == 0) {
            module = modules->data[i];
        }
    }

    if (!module) {
        module = (struct Module *)calloc(1, sizeof(struct Module));
        module->inputs = (struct Modules *)calloc(1, sizeof(struct Modules));
        module->destinations = (struct Modules *)calloc(1, sizeof(struct Modules));

        strcpy(module->name, name);

        addModule(modules, module);
    }

    return module;
}

struct PendingPulse {
    struct Module *source;
    struct Module *destination;
    enum Pulse pulse;

    struct PendingPulse *next;
};

struct PendingPulses {
    struct PendingPulse *head;
    struct PendingPulse *tail;
};

void push(struct PendingPulses *pulses, struct PendingPulse *pulse) {
    if (pulses->tail) {
        pulses->tail->next = pulse;
        pulses->tail = pulse;
    } else {
        pulses->head = pulses->tail = pulse;
    }
}

struct PendingPulse *pop(struct PendingPulses *pulses) {
    struct PendingPulse *pulse = pulses->head;

    if (pulses->tail == pulse) {
        pulses->head = pulses->tail = NULL;
    } else if (pulse) {
        pulses->head = pulse->next;
    }

    return pulse;
}

struct InputPress {
    char name[16];
    int presses;
};

struct InputPresses {
    int inputCount;
    struct InputPress *data;

    int pressCount;
    bool allSet;
};

void addInputPress(struct InputPresses *presses, struct InputPress press) {
    presses->data = (struct InputPress *)realloc(presses->data, (presses->inputCount + 1) * sizeof(struct InputPress));
    presses->data[presses->inputCount++] = press;
}

long gcd(long a, long b) {
    long temp;

    while (b != 0) {
        temp = b;
        b = a % b;
        a = temp;
    }

    return a;
}

long lcm(long a, long b) {
    return a * b / gcd(a, b);
}

void pushButton(struct Modules *modules, int *lowCount, int *highCount, struct InputPresses *inputPresses) {
    struct PendingPulse *buttonPulse = (struct PendingPulse *)calloc(1, sizeof(struct PendingPulse));
    buttonPulse->source = getModule(modules, "button");
    buttonPulse->destination = getModule(modules, "broadcaster");
    buttonPulse->pulse = LOW;

    struct PendingPulses *pulses = (struct PendingPulses *)calloc(1, sizeof(struct PendingPulses));

    push(pulses, buttonPulse);

    struct PendingPulse *pendingPulse = pop(pulses);

    if (inputPresses) {
        ++inputPresses->pressCount;
    }

    while (pendingPulse) {
        struct Module *module = pendingPulse->destination;
        enum Pulse pulse = pendingPulse->pulse;
        bool sendPulse = true;
        enum Pulse destinationPulse = LOW;
        bool allHigh = true;

        if (lowCount && highCount) {
            if (pulse == LOW) {
                ++*lowCount;
            } else {
                ++*highCount;
            }
        }

        if (module->type == FLIP_FLOP) {
            if (pulse == LOW) {
                if (module->state == OFF) {
                    module->state = ON;
                    destinationPulse = HIGH;
                } else if (module->state == ON) {
                    module->state = OFF;
                    destinationPulse = LOW;
                }
            } else {
                sendPulse = false;
            }
        } else if (module->type == CONJUNCTION) {
            for (int j = 0; allHigh && j < module->inputs->count; j++) {
                allHigh = module->inputs->data[j]->pulse == HIGH;
            }

            destinationPulse = allHigh ? LOW : HIGH;
        }

        if (sendPulse) {
            for (int i = 0; i < module->destinations->count; i++) {
                struct Module *destination = module->destinations->data[i];

                struct PendingPulse *pendingPulse = (struct PendingPulse *)calloc(1, sizeof(struct PendingPulse));
                pendingPulse->source = module;
                pendingPulse->destination = destination;
                pendingPulse->pulse = destinationPulse;

                if (destinationPulse == LOW && inputPresses) {
                    inputPresses->allSet = true;

                    for (int j = 0; j < inputPresses->inputCount; j++) {
                        if (strcmp(inputPresses->data[j].name, destination->name) == 0) {
                            inputPresses->data[j].presses = inputPresses->pressCount;
                        }

                        inputPresses->allSet = inputPresses->allSet && inputPresses->data[j].presses > 0;
                    }
                }

                if (destination->type == CONJUNCTION) {
                    pendingPulse->source->pulse = destinationPulse;
                }

                push(pulses, pendingPulse);
            }
        }

        free(pendingPulse);

        pendingPulse = pop(pulses);
    }

    free(pulses);
}

struct Modules *getModules(char *input) {
    FILE *inputFile = fopen(input, "r");
    struct Modules *modules = NULL;

    if (inputFile) {
        char line[64];
        struct Module *module;
        char *name;
        enum ModuleType type;
        struct Module *destination;
        int lowCount = 0;
        int highCount = 0;
        long answer = 0;

        modules = (struct Modules *)calloc(1, sizeof(struct Modules));

        struct Module *button = (struct Module *)calloc(1, sizeof(struct Module));
        strcpy(button->name, "button");
        button->type = BUTTON;

        addModule(modules, button);

        while(!feof(inputFile) && fgets(line, sizeof(line), inputFile)) {
            line[strcspn(line, "\n")] = 0;

            name = strtok(line, " ->,");

            switch (*name) {
            case '%':
                type = FLIP_FLOP;
                break;
            case '&':
                type = CONJUNCTION;
                break;
            case 'b':
                type = BROADCAST;
                break;
            default:
                break;
            }

            while (!isalpha(*name)) {
                ++name;
            }

            module = getModule(modules, name);
            module->type = type;

            name = strtok(NULL, " ->,");

            while (name) {
                destination = getModule(modules, name);

                addModule(module->destinations, destination);
                addModule(destination->inputs, module);

                name = strtok(NULL, " ,");
            } 
        }

        fclose(inputFile);
    }

    return modules;
}

void freeModules(struct Modules *modules) {
    for (int i = 0; i < modules->count; i++) {
        if (modules->data[i]->inputs) {
            if (modules->data[i]->inputs->data) {
                free(modules->data[i]->inputs->data);
            }

            free(modules->data[i]->inputs);
        }

        if (modules->data[i]->destinations) {
            if (modules->data[i]->destinations->data) {
                free(modules->data[i]->destinations->data);
            }

            free(modules->data[i]->destinations);
        }

        free(modules->data[i]);
    }

    free(modules);
}