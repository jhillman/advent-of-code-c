/* Day 7, part 2 = 929 */

#include "steps.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct StepData *data = getStepData(aoc.input);
    int answer = 0;

    if (data) {
        struct Worker workers[5] = { { '\0', 0 } };
        char lowest;
        int time = 0;

        while (stepsRemaining(data) || stillWorking(workers)) {
            for (int i = 0; i < 5; i++) {
                if (workers[i].timeRemaining == 0) {
                    lowest = lowestId(data);

                    if (lowest) {
                        workers[i].stepId = lowest;
                        workers[i].timeRemaining = 60 + lowest - 'A' + 1;

                        completeStep(data, lowest);

                        if (!stepsRemaining(data)) {
                            break;
                        }
                    }
                }
            }

            for (int i = 0; i < 5; i++) {
                if (workers[i].timeRemaining > 0) {
                    --workers[i].timeRemaining;

                    if (workers[i].timeRemaining == 0) {
                        for (int id = 'A'; id <= 'Z'; id++) {
                            removePrerequisite(data, id, workers[i].stepId);
                        }
                    }
                }
            }

            ++time;
        }

        answer = time;

        free(data);
    }

    print(aoc, INT, &answer);

    return 0;
}
