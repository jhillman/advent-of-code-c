/* Day 3, part 2 = 82824352 */

#include "../../aoc.h"
#include "schematic.h"
#include "gears.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Schematic *schematic = getSchematic(aoc.input);

    if (schematic) {
        struct Gears *gears = (struct Gears *)calloc(1, sizeof(struct Gears *));

        int answer = 0;

        for (int y = 0; y < schematic->height; y++) {
            for (int x = 0; x < schematic->width; x++) {
                struct Part part = getPart(schematic, x, y);

                if (part.part) {
                    x += digitCount(part.number);

                    if (part.part == '*') {
                        struct Gear *gear = getGear(gears, part.x, part.y);

                        if (gear->partCount++ < 2) {
                            gear->ratio *= part.number;
                        } 
                    }
                }
            }
        }

        for (int i = 0; i < gears->count; i++) {
            if (gears->gears[i]->partCount == 2) {
                answer += gears->gears[i]->ratio;
            }
        }

        freeGears(gears);
        freeSchematic(schematic);

        print(aoc, INT, &answer);
    }

    return 0;
}
