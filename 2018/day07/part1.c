/* Day 7, part 1 = OUGLTKDJVBRMIXSACWYPEQNHZF */

#include "steps.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct StepData *data = getStepData(aoc.input);
    char answer[27] = "-";

    if (data) {
        struct Step *step;
        char lowest;

        while (stepsRemaining(data)) {
            lowest = lowestId(data);

            for (int id = 'A'; id <= 'Z'; id++) {
                removePrerequisite(data, id, lowest);
            }

            completeStep(data, lowest);
        }

        strcpy(answer, data->completed);

        free(data);
    }

    print(aoc, STRING, answer);

    return 0;
}
