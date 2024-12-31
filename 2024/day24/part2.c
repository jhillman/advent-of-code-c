/* Day 24, part 2 = bks,hnd,nrn,tdv,tjp,z09,z16,z23 */

#include "../../aoc.h"
#include "gates.h"

void printWire(struct Wire *wire, char *prefix1, char *prefix2, char *postfix, int depth) {
    for (int i = 0; i < depth; i++) {
        printf(" ");
    }

    printf("%s%s: %s%s\n", prefix1, prefix2, wire->name, postfix);
}

void printGate(struct Gates *gates, struct Gate *gate, int depth) {
    struct Gate *output;
    char type[5] = "";

    if (gate == NULL) {
        return;
    }

    switch (gate->type) {
    case AND:
        strcpy(type, " AND");
        break;
    case OR:
        strcpy(type, " OR");
        break;
    case XOR:
        strcpy(type, " XOR");
        break;
    default:
        break;
    }

    printWire(gate->output, "o", "", type, depth);

    printWire(gate->input1, gate->output->name, " i1", "", depth + 3);
    printGate(gates, findGate(gates, gate->input1->name, 0, gates->count), depth + 8);

    printWire(gate->input2, gate->output->name, " i2", "", depth + 3);
    printGate(gates, findGate(gates, gate->input2->name, 0, gates->count), depth + 8);

    if (depth == 0) {
        printf("\n");
    }
}

void validateGate(struct Gates *gates, struct Gate *gate, enum GateType type, struct Wires *wrongWires) {
    if (gate->type != type) {
        if (!findWire(wrongWires, gate->output->name, 0, wrongWires->count)) {
            addWire(wrongWires, gate->output, true);
        }

        return;
    }

    struct Gate *input1 = findGate(gates, gate->input1->name, 0, gates->count);
    struct Gate *input2 = findGate(gates, gate->input2->name, 0, gates->count);

    if (input1 == NULL || input2 == NULL) {
        return;
    }

    switch (type) {
    case AND:
        if (input1->type == AND && input2->type == XOR) {
            validateGate(gates, input1, AND, wrongWires);
            validateGate(gates, input2, XOR, wrongWires);
        } else if (input1->type == XOR && input2->type == AND) {
            validateGate(gates, input1, XOR, wrongWires);
            validateGate(gates, input2, AND, wrongWires);
        } else if (input1->type == OR) {
            validateGate(gates, input1, OR, wrongWires);
            validateGate(gates, input2, XOR, wrongWires);
        } else if (input1->type == XOR) {
            validateGate(gates, input1, XOR, wrongWires);
            validateGate(gates, input2, OR, wrongWires);
        }
        break;
    case OR:
        validateGate(gates, input1, AND, wrongWires);
        validateGate(gates, input2, AND, wrongWires);
        break;
    case XOR:
        if (input1->type == XOR && input2->type == XOR) {
            validateGate(gates, input1, XOR, wrongWires);
            validateGate(gates, input2, XOR, wrongWires);
        } else if (input1->type == OR) {
            validateGate(gates, input1, OR, wrongWires);
            validateGate(gates, input2, XOR, wrongWires);
        } else if (input1->type == XOR) {
            validateGate(gates, input1, XOR, wrongWires);
            validateGate(gates, input2, OR, wrongWires);
        }
        break;
    default: 
        break;
    }
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    struct Gates *gates = getGates(aoc.input);
    char *answer = NULL;

    if (gates) {
        struct Wires *wrongWires = (struct Wires *)calloc(1, sizeof(struct Wires));

        for (int i = 0; i < gates->zWires->count; i++) {
            validateGate(
                gates, 
                findGate(gates, gates->zWires->data[i]->name, 0, gates->count), 
                i < gates->zWires->count - 1 ? XOR : OR,
                wrongWires
            );
        }

        answer = (char *)malloc(wrongWires->count * 4);

        for (int i = 0; i < wrongWires->count; i++) {
            sprintf(answer + strlen(answer), "%s%s", wrongWires->data[i]->name, 
                i < wrongWires->count - 1 ? "," : "");
        }

        freeWires(wrongWires, false);
        freeGates(gates);
    }

    print(aoc, STRING, answer);

    if (answer) {
        free(answer);
    }

    return 0;
}
