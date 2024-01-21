/* Day 3, part 2 = 13698 */

#include "../../aoc.h"
#include <stdlib.h>
#include "wires.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct WireData data = readInput(aoc.input);
    int answer = 0;

    if (data.path1Length > 0 && data.path2Length > 0) {
        int minSteps = -1;

        for (int i = 1; i < data.path1Length; i++) {
            struct Point wire1Point1 = data.wire1Path[i - 1];
            struct Point wire1Point2 = data.wire1Path[i];

            for (int j = 1; j < data.path2Length; j++) {
                struct Point wire2Point1 = data.wire2Path[j - 1];
                struct Point wire2Point2 = data.wire2Path[j];

                struct Intersection intersection = findIntersection((struct Line) { wire1Point1, wire1Point2 }, 
                                                                    (struct Line) { wire2Point1, wire2Point2 });

                if (intersection.valid) {
                    bool intersectionFound = false;
                    int wire1Steps = 0;

                    for (int k = 1; !intersectionFound && k < data.path1Length; k++) {
                        struct Line line = { data.wire1Path[k - 1], data.wire1Path[k] };

                        if (pointInsideLine(intersection.point, line)) {
                            intersectionFound = true;
                            wire1Steps += lineLength((struct Line) { line.point1, intersection.point });
                        } else {
                            wire1Steps += lineLength(line);
                        }
                    }

                    intersectionFound = false;
                    int wire2Steps = 0;

                    for (int k = 1; !intersectionFound && k < data.path2Length; k++) {
                        struct Line line = { data.wire2Path[k - 1], data.wire2Path[k] };

                        if (pointInsideLine(intersection.point, line)) {
                            intersectionFound = true;
                            wire2Steps += lineLength((struct Line) { line.point1, intersection.point });
                        } else {
                            wire2Steps += lineLength(line);
                        }
                    }

                    int steps = wire1Steps + wire2Steps;

                    if ((minSteps == -1 || steps < minSteps) && steps > 0) {
                        minSteps = steps;
                    }
                }
            }
        }

        freeData(data);

        answer = minSteps;
    }

    print(aoc, INT, &answer);

    return 0;
}