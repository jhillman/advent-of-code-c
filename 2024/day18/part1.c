/* Day 18, part 1 = 324 */

#include "../../aoc.h"
#include "memory.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    struct Memory *memory = getMemory(aoc.input);
    int answer = 0;

    if (memory) {
        dropBytes(memory, MIN_BYTES);
        fclose(memory->inputFile);

        answer = fewestSteps(memory);

        freeMemory(memory);
    }

    print(aoc, INT, &answer);

    return 0;
}
