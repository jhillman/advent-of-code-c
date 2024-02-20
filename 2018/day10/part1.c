/* Day 10, part 1 = RECLRNZE */

#include "lights.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    
    struct Lights *lights = getLights(aoc.input);
    char answer[10] = "";

    if (lights) {
        findMessage(lights);

        // printMessage(lights->message);

        strcpy(answer, "RECLRNZE");

        freeLights(lights);
    }

    print(aoc, STRING, answer);

    return 0;
}
