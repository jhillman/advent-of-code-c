/* Day 6, part 2 = 46966 */

#include "coordinates.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct CoordinatesList *list = getCoordinates(aoc.input);
    int answer = 0;

    if (list) {
        struct Coordinates coordinates;
        struct Coordinates *closest;
        int area = 0;

        for (int x = list->minimum.x; x <= list->maximum.x; x++) {
            for (int y = list->minimum.y; y <= list->maximum.y; y++) {
                coordinates.x = x;
                coordinates.y = y;

                int coordinateDistanceSum = 0;

                for (int i = 0; i < list->count; i++) {
                    coordinateDistanceSum += distance(coordinates, list->data[i]);
                }

                if (coordinateDistanceSum < 10000) {
                    ++area;
                }
            }            
        }

        answer = area;

        freeCoordinates(list);
    }

    print(aoc, INT, &answer);

    return 0;
}
