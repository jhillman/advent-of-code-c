/* Day 1, part 1 = 66306 */

#include "../../aoc.h"
#include "elves.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    int *elfCalories = readElfCalories(aoc.input);
    int answer = 0;

    if (elfCalories) {
        answer = *elfCalories;

        free(elfCalories);
    }

    print(aoc, INT, &answer);

    return 0;
}
