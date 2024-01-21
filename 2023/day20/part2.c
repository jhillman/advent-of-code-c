/* Day 20, part 2 = 225514321828633 */

#include "../../aoc.h"
#include "pulses.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Modules *modules = getModules(aoc.input);
    long answer = 0;

    if (modules) {
        struct Module *rxInput = *getModule(modules, "rx")->inputs->data;
        struct InputPresses *inputPresses = (struct InputPresses *)calloc(1, sizeof(struct InputPresses));
        bool findingPresses = true;

        for (int i = 0; i < rxInput->inputs->count; i++) {
            struct InputPress inputPress;
            strcpy(inputPress.name, rxInput->inputs->data[i]->name);
            inputPress.presses = 0;

            addInputPress(inputPresses, inputPress);
        }

        while (findingPresses) {
            pushButton(modules, NULL, NULL, inputPresses);

            findingPresses = !inputPresses->allSet;
        }

        for (int i = 0; i < inputPresses->inputCount; i++) {
            long presses = inputPresses->data[i].presses;

            answer = answer ? lcm(answer, presses) : presses;
        }

        free(inputPresses->data);
        free(inputPresses);

        freeModules(modules);
    }

    print(aoc, LONG, &answer);
}
