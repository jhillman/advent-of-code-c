/* Day 15, part 2 = 352 */

#include "../../aoc.h"
#include "ship.h"

#define max(a,b) ((a) > (b) ? (a) : (b))

int fillOxygen(enum LocationType **shipArea, struct Location location, int minutes) {
    enum Direction directions[] = { NORTH, SOUTH, WEST, EAST };
    int nextMinute = minutes + 1;

    shipArea[location.y][location.x] = OXYGEN;

    for (int i = 0; i < 4; i++) {
        struct Location step = update(location, directions[i]);

        if (shipArea[step.y][step.x] == PATH) {
            int stepMinutes = fillOxygen(shipArea, step, nextMinute);

            minutes = max(stepMinutes, minutes);
        }
    }

    return minutes;
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Ship *ship = getShip(aoc.input);
    int answer = 0;

    if (ship) {
        findOxygenSystem(ship);

        ship->area[ship->droidStart.y][ship->droidStart.x] = PATH;
        ship->area[ship->oxygenSystem.y][ship->oxygenSystem.x] = PATH;

        answer = fillOxygen(ship->area, ship->oxygenSystem, 0);

        freeShip(ship);
    }

    print(aoc, INT, &answer);

    return 0;
}
