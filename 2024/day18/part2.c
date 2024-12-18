/* Day 18, part 2 = 46,23 */

#include "../../aoc.h"
#include "memory.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    struct Memory *memory = getMemory(aoc.input);
    char answer[16] = "-1,-1";

    if (memory) {
        int xDeltas[] = { -1, 1,  0, 0, -1,  1, -1, 1 };
        int yDeltas[] = {  0, 0, -1, 1, -1, -1,  1, 1 };
        bool safe;

        dropBytes(memory, MIN_BYTES);

        int steps = fewestSteps(memory);

        while (steps < MAX_STEPS) {
            do {
                dropBytes(memory, 1);
            } while (!memory->visited[memory->lastByte.y][memory->lastByte.x]);

            safe = true;

            for (int i = 0; safe && i < 8; i++) {
                int x = memory->lastByte.x + xDeltas[i];
                int y = memory->lastByte.y + yDeltas[i];

                if (!(x >= 0 && x < memory->width && y >= 0 && y < memory->height)) {
                    continue;
                }

                safe = memory->grid[y][x] == SAFE;
            }

            if (!safe) {
                steps = fewestSteps(memory);
            }
        }

        fclose(memory->inputFile);

        sprintf(answer, "%d,%d", memory->lastByte.x, memory->lastByte.y);

        freeMemory(memory);
    }

    print(aoc, STRING, &answer);

    return 0;
}
