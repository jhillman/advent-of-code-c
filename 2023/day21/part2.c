/* Day 21, part 2 = 597102953699891 */

#include "../../aoc.h"
#include "farm.h"
#include <math.h>

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Farm *farm = getFarm(aoc.input, 512);
    long answer = 0;

    if (farm) {
        long maxSteps = 26501365;
        long farmSize = farm->width;
        long gridSize = maxSteps / farmSize - 1;
        struct Location edges[] = {
            { farm->start.x, 0 },
            { farm->start.x, farmSize - 1 },
            { 0, farm->start.y },
            { farmSize - 1, farm->start.y }
        };
        struct Location corners[] = {
            { 0, 0 },
            { farmSize - 1, 0 },
            { 0, farmSize - 1 },
            { farmSize - 1, farmSize - 1 }
        };

        long oddTiles = reachablePlots(farm, farm->start, farmSize * 2 + 1, 0);
        clearVisited(farm);

        long evenTiles = reachablePlots(farm, farm->start, farmSize * 2, 0);
        clearVisited(farm);

        long fullyContained = oddTiles * pow(gridSize / 2 * 2 + 1, 2) + evenTiles * pow((gridSize + 1) / 2 * 2, 2);
        clearVisited(farm);
        
        long inCorners = 0;
        long inSmallTriangleEdges = 0;
        long inLargeTriangleEdges = 0;

        for (int i = 0; i < 4; i++) {
            inCorners += reachablePlots(farm, edges[i], farmSize - 1, 0);
            clearVisited(farm);
        }

        for (int i = 0; i < 4; i++) {
            inSmallTriangleEdges += reachablePlots(farm, corners[i], farmSize / 2 - 1, 0) * (gridSize + 1);
            clearVisited(farm);
        }

        for (int i = 0; i < 4; i++) {
            inLargeTriangleEdges += reachablePlots(farm, corners[i], farmSize * 3 / 2 - 1, 0) * gridSize;
            clearVisited(farm);
        }

        answer = fullyContained + inCorners + inSmallTriangleEdges + inLargeTriangleEdges;

        freeFarm(farm);
    }

    print(aoc, LONG, &answer);

    return 0;
}
