/* Day 10, part 1 = 15020 */

#include "../../aoc.h"
#include "cpu.h"

void handleCycle(int cycle, int x, void *output) {
    int *signalStrenthSum = (int *)output;

    switch (cycle) {
    case 20:
    case 60:
    case 100:
    case 140:
    case 180:
    case 220:
        *signalStrenthSum += cycle * x;
        break;
    }
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = 0;

    runCpu(aoc.input, &answer);

    print(aoc, INT, &answer);

    return 0;
}
