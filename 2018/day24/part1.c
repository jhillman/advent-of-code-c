/* Day 24, part 1 = 25088 */

#include "simulator.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Groups *groups = getGroups(aoc.input, 0);
    int answer = 0;

    if (groups) {
        struct Group *group;

        fight(groups);

        for (int i = 0; i < groups->count; i++) {
            struct Group *group = groups->data[i];

            if (group->units > 0) {
                answer += group->units;
            }
        }

        freeGroups(groups);
    }

    print(aoc, INT, &answer);

    return 0;
}
