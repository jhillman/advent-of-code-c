/* Day 20, part 1 = 825167435 */

#include "../../aoc.h"
#include "pulses.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Modules *modules = getModules(aoc.input);
    int answer = 0;

    if (modules) {
        int lowCount = 0;
        int highCount = 0;

        for (int i = 0; i < 1000; i++) {
            pushButton(modules, &lowCount, &highCount, NULL);
        }

        answer = lowCount * highCount;

        freeModules(modules);
    }

    print(aoc, INT, &answer);
}