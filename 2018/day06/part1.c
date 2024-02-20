/* Day 6, part 1 = 4829 */

#include "coordinates.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct CoordinatesList *list = getCoordinates(aoc.input);
    int answer = 0;

    if (list) {
        struct Coordinates coordinates;
        struct Coordinates *closest;

        for (int x = list->minimum.x; x <= list->maximum.x; x++) {
            for (int y = list->minimum.y; y <= list->maximum.y; y++) {
                coordinates.x = x;
                coordinates.y = y;

                int minimumDistance = 0;

                for (int i = 0; i < list->count; i++) {
                    int coordinatesDistance = distance(coordinates, list->data[i]);

                    minimumDistance = minimumDistance ? min(minimumDistance, coordinatesDistance) : coordinatesDistance;
                }

                closest = NULL;

                for (int i = 0; i < list->count; i++) {
                    if (distance(coordinates, list->data[i]) == minimumDistance) {
                        if (closest) {
                            closest = NULL;
                            break;
                        } else {
                            closest = &list->data[i];
                        }
                    }
                }

                if (closest) {
                    if (coordinates.x == list->minimum.x || 
                        coordinates.x == list->maximum.x || 
                        coordinates.y == list->minimum.y || 
                        coordinates.y == list->maximum.y) {
                        closest->area = INFINITE;
                    } else {
                        if (closest->area != INFINITE) {
                            ++closest->area;
                        }
                    }
                }
            }            
        }

        int maxArea = 0;

        for (int i = 0; i < list->count; i++) {
            maxArea = max(maxArea, list->data[i].area);
        }

        answer = maxArea;

        freeCoordinates(list);
    }

    print(aoc, INT, &answer);

    return 0;
}
