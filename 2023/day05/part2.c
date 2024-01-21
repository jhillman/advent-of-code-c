/* Day 5, part 2 = 78775051 */

#include "../../aoc.h"
#include "garden.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Garden *garden = getGarden(aoc.input, true);

    if (garden) {
        struct Map *map;
        long value;
        long location = 0;
        long locationDelta = 10000;
        bool narrowing = false;
        long answer = 0;

        while (!answer) {
            if (location >= 0) {
                value = processValue(garden, location);

                for (int i = 0; !answer && i < garden->seeds->count; i += 2) {
                    if (value >= garden->seeds->data[i] && 
                        value < garden->seeds->data[i] + garden->seeds->data[i + 1]) {
                        if (narrowing) {
                            answer = location;
                        } else {
                            narrowing = true;
                            location -= 10000;
                            locationDelta = 1;
                        }
                    }
                }
            }

            location += locationDelta;
        }

        freeGarden(garden);

        print(aoc, LONG, &answer);
    }

    return 0;
}
