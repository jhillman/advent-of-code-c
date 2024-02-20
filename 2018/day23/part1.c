/* Day 23, part 1 = 889 */

#include "bots.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Bots *bots = getBots(aoc.input);
    int answer = 0;

    if (bots) {
        struct Bot *strongestBot = NULL;

        for (int i = 0; i < bots->count; i++) {
            if (strongestBot == NULL || bots->data[i].radius > strongestBot->radius) {
                strongestBot = &bots->data[i];
            }
        }

        for (int i = 0; i < bots->count; i++) {
            if (distanceBetween(strongestBot->location, bots->data[i].location) <= strongestBot->radius) {
                ++answer;
            }
        }

        freeBots(bots);
    }

    print(aoc, INT, &answer);

    return 0;
}