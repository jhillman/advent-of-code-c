/* Day 13, part 1 = 130,104 */

#include "mine.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Mine *mine = getMine(aoc.input);
    char answer[8] = "0,0";

    if (mine) {
        while (mine->movingCarts == mine->carts->count) {
            moveCarts(mine);
        }

        for (int i = 0; i < mine->carts->count; i++) {
            if (mine->carts->data[i].crashed) {
                sprintf(answer, "%d,%d", mine->carts->data[i].location.x, mine->carts->data[i].location.y);
                break;
            }
        }

        freeMine(mine);
    }

    print(aoc, STRING, answer);

    return 0;
}
