/* Day 3, part 1 = 870 */

#include "../../aoc.h"
#include <stdlib.h>
#include "wires.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct WireData data = readInput(aoc.input);
    int answer = 0;

    if (data.path1Length > 0 && data.path2Length > 0) {
        int minDistance = -1;

        for (int i = 1; i < data.path1Length; i++) {
            struct Point wire1Point1 = data.wire1Path[i - 1];
            struct Point wire1Point2 = data.wire1Path[i];

            for (int j = 1; j < data.path2Length; j++) {
                struct Point wire2Point1 = data.wire2Path[j - 1];
                struct Point wire2Point2 = data.wire2Path[j];

                struct Intersection intersection = findIntersection((struct Line) { wire1Point1, wire1Point2 }, 
                                                                    (struct Line) { wire2Point1, wire2Point2 });

                if (intersection.valid) {
                    int distance = abs(intersection.point.x) + abs(intersection.point.y);

                    if ((minDistance == -1 || distance < minDistance) && distance > 0) {
                        minDistance = distance;
                    }
                }
            }
        }

        freeData(data);

        answer = minDistance;
    }

    print(aoc, INT, &answer);

    return 0;
}