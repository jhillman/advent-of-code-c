#include <stdlib.h>
#include <stdbool.h>

struct Wire {
    char name[4];
    bool set;
    int value;
};

struct Wires {
    int capacity;
    int count;
    struct Wire **data;
};

int compareWires(const void *a, const void *b) {
    return strcmp((*(struct Wire **)a)->name, (*(struct Wire **)b)->name);
}

void addWire(struct Wires *wires, struct Wire *wire, bool sort) {
    if (wires->count == wires->capacity) {
        wires->capacity += 100;
        wires->data = (struct Wire **)realloc(wires->data, wires->capacity * sizeof(struct Wire *));
    }

    wires->data[wires->count++] = wire;

    if (sort) {
       qsort(wires->data, wires->count, sizeof(struct Wire *), compareWires);
    }
}

struct Wire *findWire(struct Wires *wires, char *name, int low, int high) {
    if (wires->count == 0 || low > high) {
        return NULL;
    }

    int middle = low + ((high - low) / 2);

    if (middle < wires->count) {
        int result = strcmp(name, wires->data[middle]->name);

        if (result == 0) {
            return wires->data[middle];
        } else if (result < 0) {
            return findWire(wires, name, low, middle - 1);
        } else {
            return findWire(wires, name, middle + 1, high);
        }
    }

    return NULL;
}

struct Wire *getWire(struct Wires *wires, char *name) {
    struct Wire *wire = findWire(wires, name, 0, wires->count);

    if (wire == NULL) {
        wire = (struct Wire *)calloc(1, sizeof(struct Wire));
        strcpy(wire->name, name);

        addWire(wires, wire, true);
    }

    return wire;
}

void freeWires(struct Wires *wires, bool freeData) {
    if (freeData) {
        for (int i = 0; i < wires->count; i++) {
            free(wires->data[i]);
        }
    }

    free(wires->data);
    free(wires);
}

enum GateType {
    AND,
    OR,
    XOR
};

struct Gate {
    enum GateType type;
    struct Wire *input1;
    struct Wire *input2;
    struct Wire *output;
};

struct Gates {
    int capacity;
    int count;
    struct Gate **data;
    struct Wires *wires;
    struct Wires *xWires;
    struct Wires *yWires;
    struct Wires *zWires;
};

int compareGates(const void *a, const void *b) {
    return strcmp((*(struct Gate **)a)->output->name, (*(struct Gate **)b)->output->name);
}

void addGate(struct Gates *gates, struct Gate *gate) {
    if (gates->count == gates->capacity) {
        gates->capacity += 100;
        gates->data = (struct Gate **)realloc(gates->data, gates->capacity * sizeof(struct Gate *));
    }

    gates->data[gates->count++] = gate;
}

struct Gate *findGate(struct Gates *gates, char *name, int low, int high) {
    if (gates->count == 0 || low > high) {
        return NULL;
    }

    int middle = low + ((high - low) / 2);

    if (middle < gates->count) {
        int result = strcmp(name, gates->data[middle]->output->name);

        if (result == 0) {
            return gates->data[middle];
        } else if (result < 0) {
            return findGate(gates, name, low, middle - 1);
        } else {
            return findGate(gates, name, middle + 1, high);
        }
    }

    return NULL;
}

struct Gates *getGates(char * input) {
    FILE *inputFile = fopen(input, "r");
    struct Gates *gates = NULL;

    if (inputFile) {
        char line[32];
        bool readingWires = true;
        char name[4];
        int value;
        char type[4];
        char input1[4];
        char input2[4];
        char output[4];
        struct Wire *wire;
        struct Gate *gate;

        gates = (struct Gates *)calloc(1, sizeof(struct Gates));
        gates->wires = (struct Wires *)calloc(1, sizeof(struct Wires));
        gates->xWires = (struct Wires *)calloc(1, sizeof(struct Wires));
        gates->yWires = (struct Wires *)calloc(1, sizeof(struct Wires));
        gates->zWires = (struct Wires *)calloc(1, sizeof(struct Wires));

        while (!feof(inputFile) && fgets(line, sizeof(line), inputFile)) {
            line[strcspn(line, "\n")] = 0;
            
            if (readingWires) {
                if (*line == 0) {
                    readingWires = false;
                } else {
                    sscanf(line, "%3[^: \n]: %d", name, &value);
                    wire = getWire(gates->wires, name);

                    wire->value = value;
                    wire->set = true;
                }
            } else {
                sscanf(line, "%3s %3s %3s -> %3s", input1, type, input2, output);

                gate = (struct Gate *)calloc(1, sizeof(struct Gate));

                if (strcmp(type, "AND") == 0) {
                    gate->type = AND;
                } else if (strcmp(type, "OR") == 0) {
                    gate->type = OR;
                } else if (strcmp(type, "XOR") == 0) {
                    gate->type = XOR;
                }

                gate->input1 = getWire(gates->wires, input1);
                gate->input2 = getWire(gates->wires, input2);
                gate->output = getWire(gates->wires, output);

                addGate(gates, gate);
            }
        }

        fclose(inputFile);

        qsort(gates->data, gates->count, sizeof(struct Gate *), compareGates);

        for (int i = 0; i < gates->wires->count; i++) {
            wire = gates->wires->data[i];

            switch (*wire->name) {
            case 'x':
                addWire(gates->xWires, wire, false);
                break;
            case 'y':
                addWire(gates->yWires, wire, false);
                break;
            case 'z':
                addWire(gates->zWires, wire, false);
                break;
            default:
                break;
            }
        }
    }

    return gates;
}

void freeGates(struct Gates *gates) {
    freeWires(gates->wires, true);
    freeWires(gates->xWires, false);
    freeWires(gates->yWires, false);
    freeWires(gates->zWires, false);
 
    free(gates->data);
    free(gates);
}

void reset(struct Wires *wires) {
    for (int i = 0; i < wires->count; i++) {
        wires->data[i]->value = 0;
        wires->data[i]->set = false;
    }
}

void run(struct Gates *gates) {
    int zWiresRemaining = gates->zWires->count;
    struct Gate *gate;

    while (zWiresRemaining > 0) {
        for (int i = 0; i < gates->count; i++) {
            gate = gates->data[i];

            if (gate->input1->set & gate->input2->set) {
                switch (gate->type) {
                case AND:
                    gate->output->value = gate->input1->value & gate->input2->value;
                    break;
                case OR:
                    gate->output->value = gate->input1->value | gate->input2->value;
                    break;
                case XOR:
                    gate->output->value = gate->input1->value ^ gate->input2->value;
                    break;
                }

                if (*gate->output->name == 'z' && !gate->output->set) {
                    --zWiresRemaining;
                }

                gate->output->set = true;
            }
        }            
    }
}

void setWiresValue(struct Wires *wires, long value) {
    reset(wires);

    long bit = 1L << (wires->count - 1);

    for (int i = wires->count - 1; i >= 0; i--) {
        wires->data[i]->value = value & bit ? 1 : 0;
        wires->data[i]->set = true;

        bit >>= 1;
    }
}

long getWiresValue(struct Wires *wires) {
    long value = 0;
    long bit = 1;

    for (int i = 0; i < wires->count; i++) {
        value += (bit * wires->data[i]->value);
        bit <<= 1;
    }

    return value;
}