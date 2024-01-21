/* Day 16, part 1 = 7927 */

#include "../../aoc.h"
#include "energy.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Contraption *contraption = getContraption(aoc.input);
    int answer = 0;

    if (contraption) {
        struct Beams *beams = (struct Beams *)calloc(1, sizeof(struct Beams));

        addBeam(beams, (struct Beam){ 0, 0, RIGHT, true });

        answer = energizedTiles(contraption, beams);

        freeBeams(beams);
        freeContraption(contraption);
    }

    print(aoc, INT, &answer);

    return 0;
}
