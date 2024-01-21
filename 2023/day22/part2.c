/* Day 22, part 2 = 100376 */

#include "../../aoc.h"
#include "tower.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Tower *tower = getTower(aoc.input);
    int answer = 0;

    if (tower) {
        struct Brick *brick;
        struct Brick *droppedBrick;
        struct Tower *testTower = (struct Tower *)calloc(1, sizeof(struct Tower));
        int *brickFallDistance = (int *)malloc(tower->bricks->count * sizeof(int));

        testTower->width = tower->width;
        testTower->height = tower->height;
        testTower->depth = tower->depth;

        testTower->data = (bool ***)malloc(testTower->height * sizeof(bool **));

        for (int y = 0; y < testTower->height; y++) {
            testTower->data[y] = (bool **)malloc(testTower->width * sizeof(bool *));

            for (int x = 0; x < testTower->width; x++) {
                testTower->data[y][x] = (bool *)malloc(testTower->depth * sizeof(bool));
            }
        }

        for (int i = 0; i < tower->bricks->count; i++) {
            for (int y = 0; y < testTower->height; y++) {
                for (int x = 0; x < testTower->width; x++) {
                    for (int z = 0; z < testTower->depth; z++) {
                        testTower->data[y][x][z] = tower->data[y][x][z];
                    }
                }
            }

            for (int j = 0; j < tower->bricks->count; j++) {
                brickFallDistance[j] = 0;
            }

            brick = &tower->bricks->data[i];

            updateBrickInTower(testTower, brick, false);

            for (int j = i + 1; j < tower->bricks->count; j++) {
                droppedBrick = &tower->bricks->data[j];

                updateBrickInTower(testTower, droppedBrick, false);

                do {
                    --brickFallDistance[j];

                    changeBrickDepth(droppedBrick, -1);
                } while (droppedBrick->start.z > 0 && !towerContainsBrick(testTower, droppedBrick));

                ++brickFallDistance[j];
                changeBrickDepth(droppedBrick, 1);
                updateBrickInTower(testTower, droppedBrick, true);

                if (brickFallDistance[j]) {
                    ++answer;
                }
            }

            for (int j = i + 1; j < tower->bricks->count; j++) {
                int zChange = -brickFallDistance[j];

                if (zChange) {
                    changeBrickDepth(&tower->bricks->data[j], zChange);
                }
            }
        }

        freeTower(testTower);
        free(brickFallDistance);
    }

    print(aoc, INT, &answer);

    return 0;
}
