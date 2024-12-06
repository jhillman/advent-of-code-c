/* Day 5, part 1 = 4814 */

#include "../../aoc.h"
#include "updates.h"

void processUpdate(struct Pages *pages, int *update, int length, bool valid, int *sum) {
    if (valid) {
        *sum += update[length / 2];
    }
}

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);
    int answer = 0;

    processUpdates(aoc.input, &answer);

    print(aoc, INT, &answer);

    return 0;
}
