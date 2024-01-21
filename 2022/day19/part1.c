/* Day 19, part 1 = 1650 */

#include "../../aoc.h"
#include "blueprints.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    int blueprintCount;
    struct Blueprint *blueprints = getBlueprints(aoc.input, &blueprintCount);
    int answer = 0;

    if (blueprints) {
        int result = 0;

        for (int i = 0; i < blueprintCount; i++) {
            result += (blueprints[i].id * maxGeodes(blueprints[i], 24));
        }

        free(blueprints);

        answer = result;
    }

    print(aoc, INT, &answer);

    return 0;
}
