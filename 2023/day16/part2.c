/* Day 16, part 2 = 8246 */

#include "../../aoc.h"
#include "energy.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Contraption *contraption = getContraption(aoc.input);
    int answer = 0;

    if (contraption) {
        struct Beams *beams = (struct Beams *)calloc(1, sizeof(struct Beams));

        for (int y = 0; y < contraption->height; y++) {
            addBeam(beams, (struct Beam){ 0, y, RIGHT, true });
            addBeam(beams, (struct Beam){ contraption->width - 1, y, LEFT, true });
        }

        for (int x = 0; x < contraption->width; x++) {
            addBeam(beams, (struct Beam){ x, 0, DOWN, true });
            addBeam(beams, (struct Beam){ x, contraption->height - 1, UP, true });
        }

        answer = energizedTiles(contraption, beams);

        freeBeams(beams);
        freeContraption(contraption);
    }

    print(aoc, INT, &answer);

    return 0;
}