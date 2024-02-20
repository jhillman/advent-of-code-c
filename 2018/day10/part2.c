/* Day 10, part 2 = 10007 */

#include "lights.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    
    struct Lights *lights = getLights(aoc.input);
    int answer = 0;

    if (lights) {
        findMessage(lights);

        answer = lights->message->seconds;

        freeLights(lights);
    }

    print(aoc, INT, &answer);

    return 0;
}