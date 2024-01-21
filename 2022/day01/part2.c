/* Day 1, part 2 = 195292 */

#include "../../aoc.h"
#include "elves.h"

int main(int argc, char **argv) {
    struct AoC aoc = init(argc, argv);

    int *elfCalories = readElfCalories(aoc.input);
    int answer = 0;

    if (elfCalories) {
        answer = elfCalories[0] + elfCalories[1] + elfCalories[2];

        free(elfCalories);
    }

    print(aoc, INT, &answer);

    return 0;
}