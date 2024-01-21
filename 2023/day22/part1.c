/* Day 22, part 1 = 527 */

#include "../../aoc.h"
#include "tower.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Tower *tower = getTower(aoc.input);
    int answer = 0;

    if (tower) {
        struct Brick *brick;

        for (int i = 0; i < tower->bricks->count; i++) {
            brick = &tower->bricks->data[i];

            bool supportsNoBricks = brick->supporting->count == 0;
            bool supportedBricksHaveOtherSupports = true;

            for (int j = 0; supportedBricksHaveOtherSupports && j < brick->supporting->count; j++) {
                supportedBricksHaveOtherSupports = supportedBricksHaveOtherSupports && 
                    brick->supporting->data[j].supportedBy->count > 1;
            }

            if (supportsNoBricks || supportedBricksHaveOtherSupports) {
                ++answer;
            }
        }

        freeTower(tower);
    }

    print(aoc, INT, &answer);

    return 0;
}
