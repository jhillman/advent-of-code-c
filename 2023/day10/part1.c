/* Day 10, part 1 = 6951 */

#include "../../aoc.h"
#include "tiles.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Tiles *tiles = getTiles(aoc.input);

    if (tiles) {
        int answer = 0;

        do {
            ++answer;

            tiles->stepper1 = updateStepper(tiles, tiles->stepper1);
            tiles->stepper2 = updateStepper(tiles, tiles->stepper2);
        } while (!(tiles->stepper1.current.x == tiles->stepper2.current.x && 
                   tiles->stepper1.current.y == tiles->stepper2.current.y));

        freeTiles(tiles);

        print(aoc, INT, &answer);
    }

    return 0;
}
