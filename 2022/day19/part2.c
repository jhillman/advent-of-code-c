/* Day 19, part 2 = 5824 */

#include "../../aoc.h"
#include "blueprints.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    int blueprintCount;
    struct Blueprint *blueprints = getBlueprints(aoc.input, &blueprintCount);
    int answer = 0;

    if (blueprints) {
        int result = 1;

        for (int i = 0; i < min(3, blueprintCount); i++) {
            result *= maxGeodes(blueprints[i], 32);
        }

        free(blueprints);

        answer = result;
    }

    print(aoc, INT, &answer);

    return 0;
}
