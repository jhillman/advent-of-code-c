/* Day 15, part 1 = 330 */

#include "../../aoc.h"
#include "ship.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Ship *ship = getShip(aoc.input);
    int answer = 0;

    if (ship) {
        findOxygenSystem(ship);

        answer = ship->droid->steps;

        freeShip(ship);
    }

    print(aoc, INT, &answer);

    return 0;
}
