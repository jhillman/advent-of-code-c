/* Day 24, part 2 = 2002 */

#include "simulator.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    struct Groups *groups = getGroups(aoc.input, 0);
    int answer = 0;

    if (groups) {
        int boost;
        int boostLow = 1;
        int boostHigh = 500;
        bool boostAlmostFound = false;
        bool boostFound = false;
        int immuneSystemGroups;
        int infectionGroups;
        struct Group *group;

        while (!boostFound) {
            if (boostAlmostFound) {
                ++boost;
            } else {
                boost = boostLow + ((boostHigh - boostLow) / 2);
            }

            freeGroups(groups);

            groups = getGroups(aoc.input, boost);

            fight(groups);

            immuneSystemGroups = 0;
            infectionGroups = 0;

            for (int i = 0; i < groups->count; i++) {
                struct Group *group = groups->data[i];

                if (group->units > 0) {
                    switch (group->type) {
                    case IMMUNE_SYSTEM:
                        ++immuneSystemGroups;
                        break;
                    case INFECTION:
                        ++infectionGroups;
                        break;
                    }
                }
            }

            if (boostAlmostFound) {
                boostFound = infectionGroups == 0;
            } else {
                if (infectionGroups == 1) {
                    boostAlmostFound = true;
                } else if (infectionGroups > 1) {
                    boostLow = boost + 1;
                } else {
                    boostHigh = boost - 1;
                }
            }
        }

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
