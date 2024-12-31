/* Day 24, part 1 = 53190357879014 */

#include "../../aoc.h"
#include "gates.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    struct Gates *gates = getGates(aoc.input);
    long answer = 0;

    if (gates) {
        run(gates);

        answer = getWiresValue(gates->zWires);

        freeGates(gates);
    }

    print(aoc, LONG, &answer);

    return 0;
}
