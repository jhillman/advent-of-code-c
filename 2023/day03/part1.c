/* Day 3, part 1 = 556057 */

#include "../../aoc.h"
#include "schematic.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Schematic *schematic = getSchematic(aoc.input);

    if (schematic) {
        int answer = 0;

        for (int y = 0; y < schematic->height; y++) {
            for (int x = 0; x < schematic->width; x++) {
                struct Part part = getPart(schematic, x, y);

                if (part.part) {
                    x += digitCount(part.number);
                    answer += part.number;
                }
            }
        }

        freeSchematic(schematic);

        print(aoc, INT, &answer);
    }

    return 0;
}
