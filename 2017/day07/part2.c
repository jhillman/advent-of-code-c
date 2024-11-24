/* Day 7, part 2 = 1206 */

#include "../../aoc.h"
#include "programs.h"
#include <stdbool.h>

int programWeight(struct Program *program) {
    int weight = program->weight;

    if (program->programs) {
        for (int i = 0; i < program->programs->count; i++) {
            weight += programWeight(program->programs->data[i]);
        }
    }

    return weight;
}

bool programBalanced(struct Program *program) {
    if (program->programs == NULL || program->programs->count == 0) {
        return true;
    }

    int weight;
    bool balanced = true;

    for (int i = 0; balanced && i < program->programs->count; i++) {
        if (i == 0) {
            weight = programWeight(program->programs->data[i]);
        } else {
            balanced = weight == programWeight(program->programs->data[i]);
        }
    }

    return balanced;
}

int compareWeights(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}

struct Program *unbalancedProgram(struct Program *program) {
    if (program->programs == NULL || program->programs->count == 0) {
        return NULL;
    }

    int weightCount = program->programs->count;
    int *weights = (int *)malloc(weightCount * sizeof(int));
    int *sortedWeights = (int *)malloc(weightCount * sizeof(int));

    for (int i = 0; i < weightCount; i++) {
        weights[i] = programWeight(program->programs->data[i]);
        sortedWeights[i] = weights[i];
    }

    qsort(sortedWeights, weightCount, sizeof(int), compareWeights);

    int middle = weightCount / 2;
    int wrongWeight;

    if (sortedWeights[0] == sortedWeights[middle]) {
        wrongWeight = sortedWeights[weightCount - 1];
    } else {
        wrongWeight = sortedWeights[0];
    }

    struct Program *unbalanced = NULL;

    for (int i = 0; unbalanced == NULL && i < weightCount; i++) {
        if (weights[i] == wrongWeight) {
            unbalanced = program->programs->data[i];
        }
    }

    return unbalanced;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    struct Programs *programs = getPrograms(aoc.input);
    int answer = 0;

    if (programs) {
        struct Program *unbalanced = programs->root;

        while (!programBalanced(unbalanced)) {
            unbalanced = unbalancedProgram(unbalanced);
        }

        struct Program *balanced = NULL;

        for (int i = 0; balanced == NULL && i < unbalanced->holder->programs->count; i++) {
            if (unbalanced->holder->programs->data[i] != unbalanced) {
                balanced = unbalanced->holder->programs->data[i];
            }
        }

        answer = programWeight(balanced) - (programWeight(unbalanced) - unbalanced->weight);

        freePrograms(programs);
    }

    print(aoc, INT, &answer);

    return 0;
}
